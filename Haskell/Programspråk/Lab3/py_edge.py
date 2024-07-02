class Edge:
    def __init__(self, lvert , rvert, weight):
        self.lvert = lvert
        self.rvert = rvert
        self.weight = weight
        self.edge = (lvert, rvert, weight)

    def __str__(self):
        return "lvert: " + str(self.lvert) + " rvert: " + str(self.rvert) + " Weight: "+ str(self.weight)

    def get_left(self):
        return self.lvert

    def get_right(self):
        return self.rvert

    def get_weight(self):
        return self.weight

    def get_edge(self):
        return self.edge