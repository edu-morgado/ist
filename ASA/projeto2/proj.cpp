#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <list>
#include <string>

struct Link
{
    int weight, o, d, indexAdjO;
};

struct Point
{
    int id, height, overflow, visited;
    std::vector<Link> links;
};

class Graph
{

public:
    int total_capacity, n_producers, n_stations, n_links, counter;
    std::vector<int> minimunCutLinks;
    std::vector<int> minimunCutPoints;
    std::vector<Point> points;
    std::vector<int> stack;
    std::vector<int> minimumCut;
    std::vector<int> solutionStations;
    std::vector<Link> solutionLinks;

    explicit Graph(int f, int e, int t)
    {
        struct Point producer, supermarket;
        n_producers = f;
        n_stations = e;
        n_links = t;
        total_capacity = 0;
        points.reserve(f + (2 * e) + 2);
        counter = 1; // its the id of the rest of the vertices
        producer.id = 0;
        producer.height = f + 2 * e + 2;
        producer.overflow = 0;
        producer.visited = 0;
        supermarket.id = 1;
        supermarket.height = 0;
        supermarket.overflow = 0;
        supermarket.visited = 0;
        points.push_back(producer); // first two vertices, origin and destiny
        points.push_back(supermarket);
        stack.push_back(0);
        minimumCut.push_back(1); // the supermarket is always part of the minimum cut, so we add it already
    }

    void addSupplier(std::string produced)
    {
        struct Point supplier;
        struct Link link1, link2;
        counter = counter + 1;
        supplier.id = counter;
        supplier.height = 0;
        supplier.overflow = 0;
        supplier.visited = 0;
        points.push_back(supplier);
        link1.weight = std::stoi(produced);
        link1.o = 0;
        link1.d = counter;
        link2.weight = 0;
        link2.o = counter;
        link2.d = 0;
        points[0].links.push_back(link1);
        points[supplier.id].links.push_back(link2);
        addBackEdgeIndex(0, supplier.id);
        //suppliers go in first and ocupy from index 2 to n_suppliers+1 indexes inside the points vector
    }

    void addStation(std::string capacity)
    {
        struct Point station, auxstation;
        struct Link link1, link2;
        counter = counter + 1;
        station.id = counter;
        station.height = 0;
        station.overflow = 0;
        station.visited = 0;
        auxstation.id = (counter + n_stations);
        auxstation.height = 0;
        auxstation.overflow = 0;
        points[counter] = station;
        points[counter + n_stations] = auxstation;
        link1.weight = std::stoi(capacity);
        link1.o = station.id;
        link1.d = auxstation.id;
        link2.weight = 0;
        link2.o = auxstation.id;
        link2.d = station.id;
        points[station.id].links.push_back(link1);
        points[auxstation.id].links.push_back(link2);
        addBackEdgeIndex(station.id, auxstation.id);

        //stations ocupy from n_suppliers+2 to n_suppliers+n_stations+1
        //ghost station are added in order to apply capacity using an edge in between the 2 points
        //if station is vertice n, the corresponding ghost station is n+n_stations,
        //therefor the ghost stations are the indexes n_suppliers +n_stations+2 to n_supplers+2*n_stations+1
    }

    void printAllgraph()
    {
        int aux;
        unsigned int i;
        int graph = n_producers + 2 * n_stations + 2;
        for (aux = 0; aux < graph; aux++)
        {
            printf("ID-----> %d height->%d overflow->%d ", points[aux].id, points[aux].height, points[aux].overflow);

            for (i = 0; i < points[aux].links.size(); i++)
            {
                printf("\n   o->%d  d->%d  c->%d ", points[aux].links[i].o, points[aux].links[i].d, points[aux].links[i].weight);
            }
            std::cout << '\n';
        }

        // prints the information of the vertices by order
    }

    void addConnection(int o, int d, int c)
    {
        struct Link link1, link2;
        if (o == d || o == 1)
            return;
        if (o > n_producers + 1 && o < n_producers + n_stations + 2)
            o = o + n_stations;

        link1.o = o;
        link1.d = d;
        link1.weight = c;
        link2.o = d;
        link2.d = o;
        link2.weight = 0;

        points[o].links.push_back(link1);
        points[d].links.push_back(link2);
        addBackEdgeIndex(o, d);
    }

    void addBackEdgeIndex(int o, int d)
    {
        int dIndex=-1, oIndex=-1;
        unsigned int i;
        for (i = 0; i < points[o].links.size(); i++)
        {
            if (points[o].links[i].d == d)
                dIndex = i;
        }
        for (i = 0; i < points[d].links.size(); i++)
        {
            if (points[d].links[i].d == o)
                oIndex = i;
        }
        if(dIndex==-1 || oIndex==-1)
            printf("error");
        points[o].links.back().indexAdjO = oIndex;
        points[d].links.back().indexAdjO = dIndex;

        // allows us to trace back the back edge of any edge, saving that information inside the edge
    }

    void swap_index(int index)
    {
        int aux, i;
        aux = stack[index];

        for (i = index; i > 0; i--)
        {
            stack[i] = stack[i - 1];
        }
        stack[0] = aux;
    }


    void Discharge(int p)
    {
        struct Point aux;
        unsigned int i, j;
        int lowest=-1, index=-1, edge=-1;
        int destiny;
        while (points[p].overflow > 0)
        {

            for (i = 0; i < points[p].links.size(); i++)
            {
                destiny = points[p].links[i].d;
                if (points[p].links[i].weight > 0)
                {
                    lowest = destiny;
                    edge = i;
                    index = i;
                    break;
                }
            }

            for (j = index; j < points[p].links.size(); j++)
            {
                destiny = points[p].links[j].d;
                if (points[destiny].height < points[lowest].height && points[p].links[j].weight > 0)
                {
                    lowest = destiny;
                    edge = j;
                }
            }
            if (points[p].height <= points[lowest].height)
                points[p].height = points[lowest].height + 1;
            if (lowest==-1 || index==-1 || edge==-1)
                printf("error");
            Push(p, lowest, edge);
        }
        stack.erase(stack.begin());
    }

    void Push(int o, int d, int dIndex)
    {
        int oIndex, dif;
        int capacity = points[o].links[dIndex].weight;

        if (points[d].overflow == 0 && d != 1 && d != 0)
        {
            stack.push_back(d);
        }
        dif = capacity - points[o].overflow;
        oIndex = points[o].links[dIndex].indexAdjO;
        if (dif >= 0)
        {
            points[o].links[dIndex].weight = dif;
            points[d].links[oIndex].weight = points[d].links[oIndex].weight + points[o].overflow;
            points[d].overflow = points[d].overflow + points[o].overflow;
            points[o].overflow = 0;
        }
        else if (dif < 0)
        {
            points[o].overflow = points[o].overflow - points[o].links[dIndex].weight;
            points[d].overflow = points[d].overflow + points[o].links[dIndex].weight;
            points[d].links[oIndex].weight = points[d].links[oIndex].weight + points[o].links[dIndex].weight;
            points[o].links[dIndex].weight = 0;
        }
    }


    void Relabel_to_front()
    {
        unsigned int i;
        int oldh, s_counter = 0, aux, index;

        for (i = 0; i < points[0].links.size(); i++)
            s_counter += points[0].links[i].weight;
        points[0].overflow = s_counter;
        Discharge(0);
        points[0].overflow = -s_counter;
        index = 0;
        aux = stack[index];
        while (stack.empty() == false)
        {
            oldh = points[aux].height;
            Discharge(aux);
            if (points[aux].height > oldh)
            {
                swap_index(0);
            }
            aux = stack[0];
        }
    }

    void ReverseGraph()
    {
        unsigned int j;
        int aux, destiny, value, i;
        int graph = n_producers + 2 * n_stations + 2;
        for (aux = 0; aux < graph; aux++)
        {
            points[aux].visited = 1;
            for (j = 0; j < points[aux].links.size(); j++)
            {
                destiny = points[aux].links[j].d;
                if (points[destiny].visited != 1)
                {
                    i = points[aux].links[j].indexAdjO;
                    value = points[destiny].links[i].weight;
                    points[destiny].links[i].weight = points[aux].links[j].weight;
                    points[aux].links[j].weight = value;
                }
            }
        }
        //Transposes the graph to apply a DFS starting in the origin. We do so because the want the minimum cut closest to the destination possible
    }

    void simpleVisit(int index)
    {
        points[index].visited = 0;

        for (unsigned int aux = 0; aux < points[index].links.size(); aux++)
        {
            int newIndex = points[index].links[aux].d;
            if (points[newIndex].visited == 1 && points[index].links[aux].weight > 0)
            {
                points[newIndex].visited = 0;
                minimumCut.push_back(points[newIndex].id);
                simpleVisit(newIndex);
            }
        }
        // defines the edges that are part of the minimum cut
    }

    void printStack()
    {
        unsigned int i;
        printf("\nstack-->");
        for (i = 0; i < stack.size(); i++)
        {
            printf("%d ", stack[i]);
        }
        printf("\n");
    }

    void printMinimunCut()
    {
        unsigned int i;
        printf("\nMinimum cut--> ");
        for (i = 0; i < minimumCut.size(); i++)
        {
            printf("%d ", minimumCut[i]);
        }
        printf("\n");
    }

    bool notInMinimumCut(int point)
    {
        unsigned int i;
        for (i = 0; i < minimumCut.size(); i++)
        {
            if (point == minimumCut[i])
                return false;
        }
        return true;
    }

    bool isStation(int point, int destination)
    {
        if (point > (n_producers + n_stations + 1) && point < (n_producers + 2 * n_stations + 2))
            if (destination > n_producers + 1 && destination < n_producers + n_stations + 2)
                return true;
        return false;
    }

    void addStation(int station)
    {
        unsigned int i;
        bool add=true;
        for(i=0;i<solutionStations.size();i++)
            if(station==solutionStations[i])
                add=false;
        if(add)
            solutionStations.push_back(station);
    }

    void saveSolution(int point, int destination)
    {
        struct Link link;

        if (isStation(point, destination))
        {
            addStation(destination);

        }
        else
        {
            if (destination == 0)
                return;
            if (point > (n_producers + n_stations + 1) && point < (n_producers + 2 * n_stations + 2))
                point = point - n_stations;
            if (destination > (n_producers + n_stations + 1) && destination < (n_producers + 2 * n_stations + 2))
                destination = destination - n_stations;
            link.o = destination;
            link.d = point;
            solutionLinks.push_back(link);
        }
    }

    void findCriticalEdges()
    {
        unsigned int i;
        int point, destination;
        for (i = 0; i < minimumCut.size(); i++)
        {
            point = minimumCut[i];
            for (unsigned int aux = 0; aux < points[point].links.size(); aux++)
            {
                destination = points[point].links[aux].d;
                if (notInMinimumCut(destination))
                    saveSolution(point, destination);
            }
        }
    }

    void sortSolution()
    {
        unsigned int i;
        int v1, v2;
        if (solutionStations.size() > 0)
            quicksort(solutionStations, 0, solutionStations.size()-1 );

        if (solutionLinks.size() > 0)
        {
            linkQuicksort(solutionLinks, 0, solutionLinks.size() -1 );
            v1 = solutionLinks[0].o;
            v2 = 0;
            for (i = 0; i < solutionLinks.size(); i++)
            {
                if (v1 != solutionLinks[i].o)
                {
                    destinyLinkQuicksort(solutionLinks, v2, i - 1);
                    v1 = solutionLinks[i].o;
                    v2 = i;
                }

                if (i == solutionLinks.size() - 1)
                {
                    destinyLinkQuicksort(solutionLinks, v2, i);
                }
            }
        }

    }

    void printSolutionStations()
    {
        unsigned int i;
        printf("\nstations are->");
        for (i = 0; i < solutionStations.size(); i++)
            printf("%d ", solutionStations[i]);
    }

    void printSolutionLinks()
    {
        unsigned int i;
        printf("------------------------------Links----------------------------\n");
        for(i=0;i<solutionLinks.size();i++)
        {
            printf(" o-->%d d-->%d\n",solutionLinks[i].o,solutionLinks[i].d);
        }
        printf("\n");
    }


    void printSolution()
    {
        unsigned int i;
        printf("%d\n", points[1].overflow);
        if(solutionStations.size()>0)
            printf("%d",solutionStations[0]);
        for (i = 1; i < solutionStations.size(); i++)
            printf(" %d", solutionStations[i]);
        printf("\n");
        for (i = 0; i < solutionLinks.size(); i++)
            printf("%d %d\n", solutionLinks[i].o, solutionLinks[i].d);
    }

    void linkQuicksort(std::vector<Link> &Links, int L, int R)
    {
        int i, j, mid, piv;
        i = L;
        j = R;
        mid = L + (R - L) / 2;
        piv = Links[mid].o;
        while (i < R || j > L)
        {
            while (Links[i].o < piv)
                i++;
            while (Links[j].o > piv)
                j--;
            if (i <= j)
            {
                tradeLinks(Links, i, j);
                i++;
                j--;
            }
            else
            {
                if (i < R)
                    linkQuicksort(Links, i, R);
                if (j > L)
                    linkQuicksort(Links, L, j);
                return;
            }
        }
    }

    void destinyLinkQuicksort(std::vector<Link> &dLinks,int L,int R)
    {
        int i, j, mid, piv;
        i = L;
        j = R;
        mid = L + (R - L) / 2;
        piv = dLinks[mid].d;
        while (i<R || j>L) {
            while (dLinks[i].d < piv)
                i++;
            while (dLinks[j].d > piv)
                j--;
            if (i <= j) {
                tradeLinks(dLinks, i, j); //error=swap function doesnt take 3 arguments
                i++;
                j--;
            }
            else {
                if (i < R)
                    destinyLinkQuicksort(dLinks, i, R);
                if (j > L)
                    destinyLinkQuicksort(dLinks, L, j);
            return;
            }
        }
    }


    void tradeLinks(std::vector<Link> &Links, int x, int y)
    {
        struct Link temp = Links[x];
        Links[x] = Links[y];
        Links[y] = temp;
    }

    void quicksort(std::vector<int> &vec, int L, int R)
    {
        int i, j, mid, piv;
        i = L;
        j = R;
        mid = L + (R - L) / 2;
        piv = vec[mid];

        while (i < R || j > L)
        {
            while (vec[i] < piv)
                i++;
            while (vec[j] > piv)
                j--;
            if (i <= j)
            {
                trade(vec, i, j); //error=swap function doesnt take 3 arguments
                i++;
                j--;
            }
            else
            {
                if (i < R)
                    quicksort(vec, i, R);
                if (j > L)
                    quicksort(vec, L, j);
                return;
            }
        }
    }

    void trade(std::vector<int> &v, int x, int y)
    {
        int temp = v[x];
        v[x] = v[y];
        v[y] = temp;
    }
};

int main()
{
    int f, e, t;
    int o, d, c;
    int aux;
    std::string input;
    std::string word;
    struct Point source;
    source.id = 0;

    if(scanf("%d %d %d\n", &f, &e, &t)!=3)
        printf("error");

    Graph net(f, e, t);

    std::getline(std::cin, input);
    std::istringstream in(input);
    while (in >> word)
        net.addSupplier(word);
    in.clear();

    std::getline(std::cin, input);
    in.str(input);
    while (in >> word)
        net.addStation(word);
    for (aux = 0; aux < t; aux++)
    {
        if(scanf("%d %d %d", &o, &d, &c)!=3)
            printf("error");
        net.addConnection(o, d, c);
    }
    net.Relabel_to_front();
    net.ReverseGraph();
    net.simpleVisit(1);
    net.findCriticalEdges();
    net.sortSolution();
    net.printSolution();
    return 0 ;
}