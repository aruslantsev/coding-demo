import numpy as np
import scipy
from collections import Counter
import re
import string


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
    
    def fit(self, X):
        length = len(X)
        for tokens in X:
            self.corpus.extend(tokens)
        counts = Counter(self.corpus)
        
        self.corpus = list(set(self.corpus))
            
        length = len(X)
        for w in self.corpus:
            self.IDF[w] = np.log(length / counts[w])
        
        for i, token in enumerate(self.corpus):
            self.token2idx[token] = i
    
    def transform(self, X):
        if not self.corpus:
            raise ValueError('Call fit method first')
            
        tfidf = scipy.sparse.lil_matrix((len(X), len(self.corpus)))
        
        for i, doc in enumerate(X):
            tf = self.get_tf(doc)
            for token in tf:
                tfidf[i, self.token2idx[token]] = tf[token] * self.IDF[token]
            
        return tfidf
    
    def fit_transform(self, X):
        self.fit(X)
        return self.transform(X)
    
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
