from collections import Counter

import numpy as np


class Leaf:
    def __init__(self, y):
        counts = Counter(y)
        self.label = None
        self.label = max(counts.keys(), key=lambda x: counts[x])
    
    def predict(self, _):
        return self.label
    
    
class Node:
    def __init__(self):
        self.threshold = None
        self.num = None
        self.rleaf = None
        self.lleaf = None
        
    def predict(self, x):
        if x[self.num] <= self.threshold:
            return self.lleaf.predict(x)
        else:
            return self.rleaf.predict(x)
        
    def fit(self, x, y):
        num_features = x.shape[1]
        data = np.hstack([x, y.reshape(-1, 1)])
        y0 = y[0]
        del x, y
        
        thr = np.zeros(num_features)
        gini = np.zeros(num_features)
        
        def calc_gini(vals):
            if vals.size == 0:
                return 0
            c = Counter(vals)
            return max(c.values())
        
        # Try to find best partitioning over all features
        for i in range(num_features):
            feat_values = np.unique(data[:, i])
            thresholds = np.zeros(len(feat_values))
            coef = np.zeros(len(feat_values))

            # For all unique feature values
            for k, j in enumerate(feat_values):
                # Count max length of classes in left and in right branches,
                # take WORST (minimal) value
                g_l = calc_gini(data[data[:, i] <= j][:, -1])
                g_r = calc_gini(data[data[:, i] > j][:, -1])
                thresholds[k] = j
                coef[k] = min(g_l, g_r)
 
            # Take BEST (maximal) value for feature (worst branch must be best of all)
            thr[i] = thresholds[np.min(np.where(coef == np.max(coef)))]
            gini[i] = np.max(coef)
            
        # Find best partitioning over all features
        self.num = np.min(np.where(gini == np.max(gini)))
        self.threshold = thr[self.num]
        
        # Divide data
        data_l = data[data[:, self.num] <= self.threshold]
        data_r = data[data[:, self.num] > self.threshold]
        x_l = data_l[:, 0:-1]
        y_l = data_l[:, -1]
        x_r = data_r[:, 0:-1]
        y_r = data_r[:, -1]
        del data_l, data_r
        
        if (y_l.size == 0) or (y_r.size == 0):
            self.lleaf = Leaf(np.array([y0]))
            self.rleaf = Leaf(np.array([y0]))
            return
            
        if len(np.unique(y_l)) <= 1:
            self.lleaf = Leaf(y_l)
        else:
            self.lleaf = Node()
            self.lleaf.fit(x_l, y_l)
            
        if len(np.unique(y_r)) <= 1:
            self.rleaf = Leaf(y_r)
        else:
            self.rleaf = Node()
            self.rleaf.fit(x_r, y_r)
        return
    

class Tree:
    def __init__(self):
        self.root = Node()
        
    def fit(self, x, y):
        self.root.fit(x, y)
        return
    
    def predict(self, x):
        y = np.zeros(x.shape[0])
        for i in range(x.shape[0]):
            y[i] = self.root.predict(x[i, :])
        return y
