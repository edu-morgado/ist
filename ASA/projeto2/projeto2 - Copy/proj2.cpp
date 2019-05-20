#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <list>
#include <string>


struct Link{
    int weight,o,d;
};

struct Point{
    int id,height,overflow, visited;
    std::vector<Link> links;
};

class Graph{

    public:
    int total_capacity,n_producers,n_stations,n_links, counter;
    std::vector<int> minimunCutLinks;
    std::vector<int> minimunCutPoints;
    std::vector<Point> points;
    std::vector<int> stack;
    std::vector<int> minimumCut;
    std::vector<int> solutionStations;
    std::vector<Link> solutionLinks;

    explicit Graph(int f,int e, int t){
        struct Point producer, supermarket;
        n_producers = f;
        n_stations = e; 
        n_links = t;
        total_capacity = 0;
        points.reserve(f + (2*e) +2);
        counter=1;
        producer.id=0;
        producer.height=f+2*e+2;
        producer.overflow=0;
        producer.visited=0;
        supermarket.id=1;
        supermarket.height=0;
        supermarket.overflow=0;
        supermarket.visited=0;
        points.push_back(producer);
        points.push_back(supermarket);
        minimumCut.push_back(1); /* the supermarket is always part of the minimum cut, so we add it already*/
    }

     void addSupplier(std::string produced){
        struct Point supplier;
        struct Link link1, link2;
        counter=counter+1;
        supplier.id=counter;
        supplier.height=0;
        supplier.overflow=0;
        supplier.visited=0;
        points.push_back(supplier);
        link1.weight=std::stoi(produced);
        link1.o=0;
        link1.d=counter;
        link2.weight=0;
        link2.o=counter;
        link2.d=0;
        points[0].links.push_back(link1);
        points[supplier.id].links.push_back(link2);
        //printf("id1->%d  id2->%d  peso do arc->%d temdeser0->%d  forid->%d  temdeser0->%d  id2->%d  temdeser0->%d\n",points[0].id, points[supplier.id].id, points[0].links[1].weight,points[0].links[1].o,points[0].links[1].d,    points[counter].links[0].weight,  points[counter].links[0].o,   points[counter].links[0].d);
    }

    void addStation(std::string capacity)
    {
        struct Point station, auxstation;
        struct Link link1, link2;
        counter=counter+1;
        station.id=counter;
        station.height=0;
        station.overflow=0;
        station.visited=0;
        auxstation.id=(counter+n_stations);
        auxstation.height=0;
        auxstation.overflow=0;
        points[counter]=station;
        points[counter+n_stations]=auxstation;
        link1.weight=std::stoi(capacity);
        link1.o=station.id;
        link1.d=auxstation.id;
        link2.weight=0;
        link2.o=auxstation.id;
        link2.d=station.id;
        points[station.id].links.push_back(link1);
        points[auxstation.id].links.push_back(link2);
        
        /*
        printf("\nid-->%d %d \n\n\n",points[station.id].id,points[auxstation.id].id);
        printf("peso-->%d %d \n\n\n",points[station.id].links[0].weight,points[auxstation.id].links[0].weight); 
        printf("origem-->%d %d \n\n\n",points[station.id].links[0].o,points[auxstation.id].links[0].o);
        printf("destino-->%d %d \n\n\n",points[station.id].links[0].d,points[auxstation.id].links[0].d); 
        */
    }

    void printAllgraph(){
        int aux, i;
        int graph = n_producers+2*n_stations+2;
        for (aux = 0; aux< graph; aux++){
            printf("ID-----> %d height->%d overflow->%d ",points[aux].id,points[aux].height,points[aux].overflow);

            for(i=0;i<points[aux].links.size();i++)
            {
                printf("\n   o->%d  d->%d  c->%d ", points[aux].links[i].o,points[aux].links[i].d,points[aux].links[i].weight);
            }
            std::cout << '\n';
            
        }
    }

    void addConnection(int o, int d, int c)
    {
        struct Link link1, link2;
        if(o==d || o==1)
            return;
        if(o>n_producers+1 && o<n_producers+n_stations+2)
            o=o+n_stations;
        link1.o=o;
        link1.d=d;
        link1.weight=c;
        link2.o=d;
        link2.d=o;
        link2.weight=0;
        points[o].links.push_back(link1);
        points[d].links.push_back(link2);
    }


        
    void swap_index(int index){
        int aux, i;
        aux=stack[index];
        
        for (i = index ; i>0;i--){
            stack[i] = stack[i-1];
        }
        stack[0]=aux;
    }

    void Push(int o, int d,int dIndex)
    {
        int capacity=points[o].links[dIndex].weight;
        int oIndex, i,dif;
        for(i=0;i<points[d].links.size();i++)
        {
            if(points[d].links[i].d==points[o].id)
                oIndex=i;
        }
        dif=capacity-points[o].overflow;
        if(dif>=0)
        {   
            points[o].links[dIndex].weight=dif;
            points[d].links[oIndex].weight=points[d].links[oIndex].weight+points[o].overflow;
            points[d].overflow=points[d].overflow+points[o].overflow;
            points[o].overflow=0;
        }
        else if(dif<0)
        {
            points[o].overflow=points[o].overflow-points[o].links[dIndex].weight;
            points[d].overflow=points[d].overflow+points[o].links[dIndex].weight;
            points[d].links[oIndex].weight=points[d].links[oIndex].weight+points[o].links[dIndex].weight;
            points[o].links[dIndex].weight=0;
        }
    }   

    void Discharge(int p)
    {
        struct Point aux;
        int lowest,i, j,index , edge;
        int destiny;
        while(points[p].overflow>0)
        {
            
            for(i=0;i<points[p].links.size();i++)
            {
                destiny=points[p].links[i].d;
                if(points[p].links[i].weight>0)
                {
                    lowest=destiny;
                    edge=i;
                    index=i;
                    break;
                }
            }
            for(j=index;j<points[p].links.size();j++)
            {
                destiny=points[p].links[j].d;
                if(points[destiny].height<points[lowest].height && points[p].links[j].weight>0 )
                {
                    lowest=destiny;
                    edge=j;
                }
            }
            if(points[p].height<=points[lowest].height)
                points[p].height=points[lowest].height+1;
            Push(p,lowest,edge);
        }

    }

    void Relabel_to_front()
    {
        int i,oldh;
        int s_counter = 0;
        int graph = n_producers+2*n_stations+2;
        int aux;
        int index;

        for(i=2;i<graph;i++)
            stack.push_back(i);

        stack.push_back(-1);


        for(i=0;i<points[0].links.size();i++)
               s_counter += points[0].links[i].weight;   
        points[0].overflow = s_counter;
        Discharge(0);
        points[0].overflow= -s_counter;
        index=0;
        aux=stack[index];
        while(aux!=-1)
        {
            oldh=points[aux].height;
            Discharge(aux);
            if(points[aux].height>oldh)
            {
                swap_index(index);
                index=-1;
            }
            aux=stack[++index];
        }

    }

    void ReverseGraph()
    {
        int aux,j, destiny,value,i;
        int graph = n_producers+2*n_stations+2;
        for (aux = 0; aux< graph; aux++)
        {
            points[aux].visited=1;
            for(j=0;j<points[aux].links.size();j++)
            {
                destiny=points[aux].links[j].d;
                if(points[destiny].visited!=1)
                {
                    for(i=0;i<points[destiny].links.size();i++)
                    {
                        if(points[destiny].links[i].d==aux)
                        {
                            value=points[destiny].links[i].weight;
                            points[destiny].links[i].weight=points[aux].links[j].weight;
                        }
                    }
                    points[aux].links[j].weight=value;
                }
            }
        }
    }

  

    void simpleVisit(int index)
    {
        points[index].visited=0;

        for(unsigned int aux=0;aux<points[index].links.size();aux++)
        {    
            int newIndex=points[index].links[aux].d;
            if(points[newIndex].visited==1 && points[index].links[aux].weight>0)
            {
                points[newIndex].visited=0;
                minimumCut.push_back(points[newIndex].id);
                simpleVisit(newIndex);

            }
        }
    }

    void printMinimunCut()
    {
    
        int i;
        printf("\nMinimum cut--> ");
        for(i=0;i<minimumCut.size();i++)
        {
            printf("%d ",minimumCut[i]);
        }
        printf("\n");
    }

    bool notInMinimumCut(int point)
    {
        int i;
        for(i=0;i<minimumCut.size();i++)
        {
            if(point==minimumCut[i])
                return false;
        }
        return true;
    }

    bool isStation(int point,int destination)
    {
        if(point>(n_producers+n_stations+1) && point<(n_producers+2*n_stations+2))
            if(destination>n_producers+1 && destination<n_producers+n_stations+2)
                return true;
        return false;
    }

    void saveSolution(int point, int destination)
    {
        struct Link link;

        if(isStation(point, destination)){
            solutionStations.push_back(destination);

        }
        else 
        {
            if(destination==0)
                return;
            if(point>(n_producers+n_stations+1) && point<(n_producers+2*n_stations+2))
                point=point-n_stations;
            if(destination>(n_producers+n_stations+1) && destination<(n_producers+2*n_stations+2))
                destination=destination-n_stations;
            link.o=destination;
            link.d=point;
            solutionLinks.push_back(link);
        }
    }

    void findCriticalEdges()
    {
        int i,aux,point,destination;
        for(i=0;i<minimumCut.size();i++)
        {
            point=minimumCut[i];
            for(unsigned int aux=0;aux<points[point].links.size();aux++)
            {
                destination=points[point].links[aux].d;
                if(notInMinimumCut(destination))
                    saveSolution(point,destination);
            }
        }
    }

    void sortSolution()
    {
        int i;
        int v1;
        if(solutionStations.size()>0)
            quicksort(solutionStations,0,solutionStations.size()-1);
        if(solutionLinks.size()>0)
            linkQuicksort(solutionLinks,0,solutionLinks.size()-1);
        v1=solutionLinks[0].o;
        printf("v1 starts at->%d",v1);
        for(i=1;i<solutionLinks.size();i++)
        {
            printf("\nsolutionLinks[i].o-->%d",solutionLinks[i].o);
            if(v1!=solutionLinks[i].o)
            {
                destinyLinkQuicksort(solutionLinks,v1,i-1);

                if(i<solutionLinks.size()-1)
                {
                    v1=solutionLinks[i].o;
                    printf("\nnew v1 value-->%d",v1);

                }
            }
        }
    }

    void printSolution()
    {
        int i;
        printf("%d\n",points[1].overflow);
        for(i=0;i<solutionStations.size();i++)
            printf("%d ",solutionStations[i]);
        printf("\n");
        for(i=0;i<solutionLinks.size();i++)
            printf("%d %d\n",solutionLinks[i].o, solutionLinks[i].d);
    }


    void linkQuicksort(std::vector<Link> &Links,int L,int R)
    {
        int i, j, mid, piv;
        i = L;
        j = R;
        mid = L + (R - L) / 2;
        piv = Links[mid].o;
        while (i<R || j>L) {
            while (Links[i].o < piv)
                i++;
            while (Links[j].o > piv)
                j--;
            if (i <= j) {
                tradeLinks(Links, i, j); 
                i++;
                j--;
            }
            else {
                if (i < R)
                    linkQuicksort(Links, i, R);
                if (j > L)
                    linkQuicksort(Links, L, j);
            return;
            }
        }
    }
    void tradeLinks(std::vector<Link>&Links, int x, int y) {
        struct Link temp = Links[x];
        Links[x] = Links[y];
        Links[y] = temp;
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
                    linkQuicksort(dLinks, i, R);
                if (j > L)
                    linkQuicksort(dLinks, L, j);
            return;
            }
        }
    }

    void quicksort(std::vector<int>&vec, int L, int R) {
        int i, j, mid, piv;
        i = L;
        j = R;
        mid = L + (R - L) / 2;
        piv = vec[mid];

        while (i<R || j>L) {
            while (vec[i] < piv)
                i++;
            while (vec[j] > piv)
                j--;

            if (i <= j) {
                trade(vec, i, j); //error=swap function doesnt take 3 arguments
                i++;
                j--;
            }
            else {
                if (i < R)
                    quicksort(vec, i, R);
                if (j > L)
                    quicksort(vec, L, j);
            return;
            }
        }
    }


    void trade(std::vector<int>& v, int x, int y) {
        int temp = v[x];
        v[x] = v[y];
        v[y] = temp;

    }



};

int main(){

    int f,e,t;
    int o,d,c;
    int aux;
    std::string input;
    std::string word;
    struct Point source;
    source.id=0;

    scanf("%d %d %d\n",&f,&e,&t);

    Graph net(f,e,t);

    std::getline(std::cin,input);
    std::istringstream in(input);
    while(in >> word)
        net.addSupplier(word);
    in.clear();


    std::getline(std::cin,input);
    in.str(input);
    while(in >> word)
        net.addStation(word);
    for(aux=0;aux<t;aux++)
    {
        scanf("%d %d %d",&o,&d,&c);
        net.addConnection(o,d,c);
    }
    net.Relabel_to_front();
    net.ReverseGraph();
    net.simpleVisit(1);
    net.findCriticalEdges();
    net.sortSolution();
    net.printSolution();
   return 1;
}