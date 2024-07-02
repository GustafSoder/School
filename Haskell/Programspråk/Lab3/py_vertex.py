class Vertex:
    def __init__(self, id, weight):
        self.id = id
        self.weight = weight
        self.vertex = (self.id, self.weight)

    def __str__(self):
        return "Id: " + str(self.id) + ", weight: " + str(self.weight)
        
    def get_weight(self):
        return self.weight

    def get_ID(self):
        return self.id
        
    def get_vertex(self):
        return self.vertex