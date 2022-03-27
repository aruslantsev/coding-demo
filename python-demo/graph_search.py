#!/usr/bin/python3

from numpy import inf

nodes = ('A', 'B', 'C', 'D', 'E', 'F')
edges = {
    'A': {'B': 10, 'C': 1, 'E': 7},
    'B': {'A': 10, 'C': 2, 'D': 7},
    'C': {'A': 1, 'B': 2, 'D': 6, 'F': 3},
    'D': {'B': 7, 'C': 6, 'F': 3},
    'E': {'A': 7, 'F': 4},
    'F': {'C': 3, 'E': 4} 
    }

# Непосещенные вершины
unvisited = {node: None for node in nodes}
# Минимальные расстояния от стартовой вершины
minDistance = {node: inf for node in nodes}
# Вершины, из которых достигаются наименьшие пути
minDistancePrev = {node: None for node in nodes}
startNode = 'A'
endNode = 'D'
currentNode = startNode
del unvisited[currentNode]
currentDistance = 0
minDistance[currentNode] = 0
minDistancePrev[currentNode] = currentNode

def graph_search(currentNode):
    # Не люблю глобальные переменные, но все же...
    global unvisited, minDistance, minDistancePrev
    # Посмотрим все соседние вершины
    for node, distance in edges[currentNode].items():
        if node in unvisited:
            # Если еще не посещали, то удаляем из непосещенных и рекурсивно просматриваем их
            del unvisited[node]
            minDistance[node] = distance + minDistance[currentNode]
            minDistancePrev[node] = currentNode
            graph_search(node)
        else:
            # Если посещали, то остается 2 варианта
            if (minDistance[node] > distance + minDistance[currentNode]):
                # Расстояние из текущей вершины меньшее, чем определяли раньше. 
                minDistance[node] = distance + minDistance[currentNode]
                minDistancePrev[node] = currentNode
                # Тогда надо обновить минимальное расстояние и расстояние до соседей
                graph_search(node)
            # Если расстояние больше, то оставляем все без изменений
    return

graph_search(currentNode)

path = []
node = endNode
while ((node != startNode) and (node != None)):
    path.insert(0, node)
    node = minDistancePrev[node]
path.insert(0, startNode)
if (node == None):
    print('No path')
else:
    print('Shortest way from', startNode, 'to', endNode)
    print(path)
    print('Path length:', minDistance[endNode])
#print('Min distances and nodes')
#print(minDistance)
#print(minDistancePrev)
