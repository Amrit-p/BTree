#!/usr/bin/python3

class Key:
    def __init__(self, value):
        self.value = value

    def __lt__(self, other):
        return self.value < other.value

    def __le__(self, other):
        return self.value <= other.value

    def __gt__(self, other):
        return self.value > other.value

    def __ge__(self, other):
        return self.value >= other.value

    def __eq__(self, other):
        return self.value == other.value

    def __str__(self):
        return str(self.value)

    def __repr__(self):
        return f"Key({self.value})"


class Node:
    def __init__(self, max_keys):
        self.keys = []
        self.children = []
        self.max_keys = max_keys
        self.is_leaf = True

    def is_full(self):
        """Returns True if the node has reached max capacity."""
        return len(self.keys) == self.max_keys

    def insert(self, key):
        """Inserts a key in the correct sorted position."""
        if self.is_full():
            mid = len(self.keys)//2
            firsthalf = self.keys[:mid]
            secondhalf = self.keys[mid:]
            node = Node(self.max_keys)
            
            print(firsthalf)
            print(secondhalf)

        self.keys.append(key)
        self.keys.sort()

    def __str__(self):
        """String representation of the node for debugging."""
        return f"Node(keys={self.keys}, is_leaf={self.is_leaf})"

    def __repr__(self):
        """Debug representation of the node."""
        return f"Node({self.keys})"

tree=Node(1)

tree.insert(Key(8))
tree.insert(Key(3))