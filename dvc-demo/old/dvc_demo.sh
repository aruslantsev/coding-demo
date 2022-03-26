#!/bin/bash

# create empty repo
rm -rf dvc
mkdir dvc
cd dvc/
git init

# add python files
echo "import pandas as pd
import numpy as np

def get_features(dataset):
    features = dataset.copy()

    # features['sepal_length_to_sepal_width'] = features['sepal_length'] / features['sepal_width']
    # features['petal_length_to_petal_width'] = features['petal_length'] / features['petal_width']
    np.random.seed(42)
    features['is_train'] = np.random.binomial(1, 0.8, size=(len(features),))
    return features


if __name__ == '__main__':
    dataset = pd.read_csv('data/iris.csv')
    features = get_features(dataset)
    features.to_csv('data/iris_featurized.csv', index=False)
" > gen_features.py

echo "import pandas as pd
import pickle
from sklearn.ensemble import RandomForestClassifier

def train(dataset):
    model = RandomForestClassifier(n_estimators=1000, random_state=42)
    X = dataset.drop(columns=['species'])
    y = dataset['species']
    model.fit(X=X, y=y)
    return model

if __name__ == '__main__':
    df = pd.read_csv('data/iris_featurized.csv')
    df = df[df['is_train'] == 1].drop(columns=['is_train'])
    model = train(df)
    with open('model.pkl', 'wb') as f:
        pickle.dump(model, f)
" > train_model.py

echo "import pandas as pd
import pickle
from sklearn.metrics import accuracy_score

def test(models, dataset):
    X = dataset.drop(columns=['species'])
    y_true = dataset['species']
    y_pred = model.predict(X=X)
    metrics = accuracy_score(y_true, y_pred)
    return metrics

if __name__ == '__main__':
    df = pd.read_csv('data/iris_featurized.csv')
    df = df[df['is_train'] == 0].drop(columns=['is_train'])
    with open('model.pkl', 'rb') as f:
        model = pickle.load(f)
    metrics = test(model, df)
    with open('metrics.txt', 'w') as f:
        f.write(f'Accuracy score: {metrics}\n')
" > eval_model.py

git add gen_features.py train_model.py eval_model.py

# init dvc
dvc init
git status
# commit dvc files
git commit -m 'dvc init'

# download dataset, add it to dvc
wget -P data/ https://raw.githubusercontent.com/uiuc-cse/data-fa14/gh-pages/data/iris.csv
dvc add data/iris.csv
# dvc asks to run git
git add data/.gitignore data/iris.csv.dvc

# gen features
dvc run -d data/iris.csv -d gen_features.py -o data/iris_featurized.csv -f gen_features.dvc python gen_features.py
# dvc asks to run git
git add data/.gitignore gen_features.dvc

# train model
dvc run -d train_model.py -d data/iris_featurized.csv -o model.pkl -f train_model.dvc python train_model.py
# dvc asks to run git
git add .gitignore train_model.dvc

# get model scores
dvc run -d eval_model.py -d data/iris_featurized.csv -d model.pkl -m metrics.txt -f eval_model.dvc python eval_model.py
# dvc asks to run git
git add .gitignore eval_model.dvc

# show dependencies, press q to exit
dvc pipeline show --ascii eval_model.dvc
# show dependencies, press q to exit
dvc pipeline show --ascii --commands train_model.dvc
# Status and metrics. All pipelines are up-to-date, dvc repro will not run anything
dvc status
dvc metrics show
dvc repro eval_model.dvc
# Add empty line to file and reproduce. DVC restarts pipeline, but output doesn't chenge
echo >> eval_model.py
dvc status
dvc repro eval_model.dvc
# Now we uncomment lines in gen_features.py and reproduce pipeline to get metrics. DVC will restart all dependencies of eval_model.dvc and eval_model.dvc
sed -i 's/\#\ //g' gen_features.py
dvc repro eval_model.dvc
dvc metrics show
