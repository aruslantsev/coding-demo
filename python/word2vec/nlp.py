import re
import string
from collections import Counter

import numpy as np
import scipy
import torch
from tqdm import tqdm


class Word2VecNN(torch.nn.Module):
    def __init__(self, corpus_size, emb_size=128):
        super().__init__()
        self.linear0 = torch.nn.Linear(in_features=corpus_size, out_features=emb_size)
        self.linear1 = torch.nn.Linear(in_features=emb_size, out_features=corpus_size)

    def forward(self, X):
        hidden = self.linear0(X)
        hidden = torch.nn.functional.relu(hidden)
        pred = self.linear1(hidden)
        return hidden, pred


class Word2Vec:
    def __init__(self):
        self.embedding_size = None
        self.corpus = []
        self.idx2token = {}
        self.token2idx = {}
        self.w2v = None

    def collect_train(self, data, window_size, train_size=None):
        # CBOW
        x = []
        y = []
        v_len = len(self.corpus)
        for row in tqdm(data):
            for i in range(len(row)):
                word = torch.zeros((1, v_len))
                if row[i] not in self.token2idx:
                    continue

                word[0, self.token2idx[row[i]]] = 1

                target = torch.zeros((1, v_len))

                for j in range(1, window_size + 1):
                    if (0 <= i - j < len(row)) and (row[i - j] in self.token2idx):
                        target[0, self.token2idx[row[i - j]]] = 1
                    if (0 <= i + j < len(row)) and (row[i + j] in self.token2idx):
                        target[0, self.token2idx[row[i + j]]] = 1

                if target.sum() == 0:
                    continue

                x.append(word)
                y.append(target)

                if train_size is not None and len(x) >= train_size:
                    break
            if train_size is not None and len(x) >= train_size:
                break

        x = torch.vstack(x)
        y = torch.vstack(y)
        print(f"Collected {x.shape[0]} train samples")
        return x, y

    def fit(self, x, embedding_size=128,
            window_size=5, epochs=10, lr=0.1, vocab_size=1000, train_size=500):
        self.embedding_size = embedding_size
        # collect statistics
        for row in x:
            self.corpus.extend(row)

        counts = dict(Counter(self.corpus))

        if vocab_size is not None and vocab_size < len(self.corpus):
            self.corpus = sorted(
                counts.keys(), key=lambda tok: counts[tok], reverse=True)[:vocab_size]
        else:
            self.corpus = list(set(counts.keys()))

        for i, token in enumerate(self.corpus):
            self.idx2token[i] = token
            self.token2idx[token] = i

        # here we go
        x, y = self.collect_train(x, window_size, train_size)

        self.w2v = Word2VecNN(len(self.corpus), self.embedding_size)
        optimizer = torch.optim.SGD(self.w2v.parameters(), lr=lr)
        loss_fn = torch.nn.BCEWithLogitsLoss()

        for e in tqdm(range(epochs)):
            optimizer.zero_grad()

            _, y_pred = self.w2v(x)

            loss = loss_fn(y_pred, y)

            loss.backward()
            optimizer.step()
            if e % (epochs // 10) == 0:
                tqdm.write(f"Epoch {e}: loss {loss.item()}")

        print(f"Loss {loss.item()}")

    def _check_trained(self):
        if self.w2v is None:
            raise Exception('Estimator is not fitted')

    def predict_string(self, w):
        self._check_trained()
        if w in self.corpus:
            wv = torch.zeros(1, len(self.corpus))
            wv[0, self.token2idx[w]] = 1
            hidden, _ = self.w2v(wv)
            return hidden.detach().cpu().numpy()
        else:
            return None

    def predict(self, x):
        self._check_trained()
        if isinstance(x, str):
            return self.predict_string(x)
        if isinstance(x, list) or isinstance(x, tuple):
            return [self.predict_string(w) for w in x]

    def context_words(self, w, n=5):
        self._check_trained()
        if w in self.corpus:
            wv = torch.zeros(1, len(self.corpus))
            wv[0, self.token2idx[w]] = 1
            _, context_words = self.w2v(wv)
            context_words = torch.nn.functional.softmax(context_words)
            topk = torch.topk(context_words, k=n)
            ind = topk.indices.detach().cpu().numpy().ravel()
            val = topk.values.detach().cpu().numpy().ravel()
            result = {}
            for i, v in zip(ind, val):
                result[self.idx2token[i]] = v
            return result
        else:
            return None

    def nearest_words(self, ):  # w, n=3):
        self._check_trained()
        raise Exception("Not implemented yet")
        # find all hiddens for corpus, find top k nearest

    def save(self, path):
        self._check_trained()
        model_state = {
            'w2v': self.w2v.model_state(),
            'corpus': self.corpus,
            'embedding_size': self.embedding_size,
            'idx2token': self.idx2token,
            'token2idx': self.token2idx,
        }
        torch.save(model_state, path)

    def load(self, path):
        model_state = torch.load(path, map_location="cpu")

        self.w2v.load_state_dict(model_state['w2v'])
        self.corpus = model_state['corpus']
        self.embedding_size = model_state['embeffing_size']
        self.idx2token = model_state['idx2token']
        self.token2idx = model_state['token2idx']


def levenshtein(s1, s2):
    DEL = 1
    INS = 1
    SWAP = 1

    dist = np.zeros((len(s1) + 1, len(s2) + 1), dtype=int)

    for i in range(1, len(s1) + 1):
        dist[i, 0] = dist[i - 1, 0] + INS

    for j in range(1, len(s2) + 1):
        dist[0, j] = dist[0, j - 1] + INS

    for i in range(1, len(s1) + 1):
        for j in range(1, len(s2) + 1):
            dist[i, j] = min(
                dist[i, j - 1] + INS,
                dist[i - 1, j] + DEL,
                dist[i - 1, j - 1] + (SWAP if s1[i - 1] != s2[j - 1] else 0)
            )

    return dist[-1, -1]


class Tokenizer:
    def __init__(self):
        self.table = str.maketrans(
            dict.fromkeys(string.punctuation))

    def tokenize(self, s):
        clean = s.translate(self.table)
        clean = re.sub("\ +", " ", clean)
        tokens = clean.replace('\n', ' ').replace('\t', ' ').lower().split(' ')
        tokens = [token for token in tokens if token != ""]
        return tokens

    def __call__(self, X):
        tokens = []
        if isinstance(X, str):
            return self.tokenize(X)

        if isinstance(X, list):
            for row in X:
                str_tokens = self.tokenize(row)
                tokens.append(str_tokens)
            return tokens

        raise ValueError('List of strings or string should be passed')


class TfIdf:
    def __init__(self):
        self.IDF = {}
        self.corpus = []
        self.token2idx = {}

    def get_tf(self, doc):
        length = len(doc)
        counts = Counter(doc)

        idf = {}
        for w in counts:
            if w in self.corpus:
                idf[w] = np.log(length / counts[w])

        return idf

    def fit(self, x):
        for tokens in x:
            self.corpus.extend(tokens)
        counts = Counter(self.corpus)

        self.corpus = list(set(self.corpus))

        length = len(x)
        for w in self.corpus:
            self.IDF[w] = np.log(length / counts[w])

        for i, token in enumerate(self.corpus):
            self.token2idx[token] = i

    def transform(self, x):
        if not self.corpus:
            raise ValueError('Call fit method first')

        tfidf = scipy.sparse.lil_matrix((len(x), len(self.corpus)))

        for i, doc in enumerate(x):
            tf = self.get_tf(doc)
            for token in tf:
                tfidf[i, self.token2idx[token]] = tf[token] * self.IDF[token]

        return tfidf

    def fit_transform(self, x):
        self.fit(x)
        return self.transform(x)

    def save(self, path):
        params = {
            "IDF": self.IDF,
            "corpus": self.corpus,
            "index": self.token2idx
        }
        np.save(path, params)

    def load(self, path):
        params = np.load(path, allow_pickle=True).item()
        self.IDF = params['IDF']
        self.corpus = params['corpus']
        self.token2idx = params['index']
