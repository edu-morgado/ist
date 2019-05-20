#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <list>



class Grafo{
public:
    int timer, narticulatedPoints, nrouters;
    std::vector<bool> articulatedPoints;
    std::vector<int> d;
    std::vector<int> low;
    std::vector<int> visited;
    std::vector<int> precedent;
    std::vector<std::vector<int> > adj;
    
    explicit Grafo(int v,int e)
    { 
        adj.reserve(v);
        narticulatedPoints=0;
        nrouters=v;
        for(int i=0;i<v;i++) 
        {
            d.push_back(-1);
            low.push_back(-1);
            articulatedPoints.push_back(false);
            visited.push_back(0);
            precedent.push_back(-1);
        } 
    }

    void add(int v1,int v2)
    {
        adj[v1-1].push_back(v2); 
        adj[v2-1].push_back(v1); 
    }

    void advancedMultipleDFS()
    {
        std::vector<int> idSubrede;
        int nSubredes=0;

        for(int i=0;i<nrouters;i++)
        {
            if(visited[i]==0)
            {
                nSubredes++;
                idSubrede.push_back(advancedDFS(i));
            }
        }

        printf("%d\n",nSubredes);

        quicksort(idSubrede,0,idSubrede.size()-1);
        for(unsigned int i=0;i<idSubrede.size()-1;i++)
        {
            printf("%d ",idSubrede[i]);
        }
        printf("%d",idSubrede[idSubrede.size()-1]);
        printf("\n");
        for(int i=0;i<nrouters;i++)
        {
            if(articulatedPoints[i])
                narticulatedPoints=narticulatedPoints+1;
        }
        printf("%d\n",narticulatedPoints);
        int routers=0;
        int aux=0;
        for(int i=0;i<nrouters;i++)
        {
            aux=simpleDFS(i);
            if(routers<aux)
                routers=aux;
        }
        printf("%d\n",routers);

        
    }

    int advancedDFS(int index)
    {
        int id=index+1;
        timer=0;
        if(visited[index]==0)
        {
            advancedVisit(index,&id);
        }

        return id;
    }   

    void advancedVisit(int index, int *id)
    {
        visited[index]=1;
        d[index]=timer;
        low[index]=timer;
        timer=timer+1;
        int children=0;

        for(unsigned int aux=0;aux<adj[index].size();aux++)
        {
            int newIndex=adj[index][aux]-1;
            int val=adj[index][aux];
            if(visited[newIndex]==0)
            {
                if (val>*id)
                    *id=val;
                children++;
                precedent[newIndex]=index+1;
                advancedVisit(newIndex,id);
                if(low[newIndex]<low[index])
                    low[index]=low[newIndex]; 
 
                if(precedent[index]==-1 && children>1)
                    articulatedPoints[index]=true;

                if (precedent[index]!= -1 && low[newIndex] >= d[index])
                    articulatedPoints[index] = true; 
                
            } 
            else if (val != precedent[index]) 
            {
                if(d[val-1]<low[index])
                    low[index]=d[val-1];
            }
        }


    }

    int simpleDFS(int index)
    {
        int nrouters=0;
        if(visited[index]==1 && articulatedPoints[index]==false)
            simpleVisit(index,&nrouters);
        return nrouters;
    }

    void simpleVisit(int index, int *nrouters)
    {
        visited[index]=0;
        *nrouters=*nrouters+1;


        for(unsigned int aux=0;aux<adj[index].size();aux++)
        {    
            int newIndex=adj[index][aux]-1;
            if(visited[newIndex]==1 && articulatedPoints[newIndex]==false)
            {
                simpleVisit(newIndex,nrouters);

            }
        }
    }
    
    void quicksort(std::vector<int> &vec, int L, int R) {
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


int main()
{
    
    int vertices, nconnections;
    int v1,v2;
    
    if(scanf("%d", &vertices)!=1)
        exit(0);
    if(scanf("%d", &nconnections)!=1)
        exit(0);
    
    Grafo routers(vertices, nconnections);

    for(int i=0;i<nconnections;i++)
    {
        if(scanf("%d %d",&v1,&v2)!=2)
            exit(0);
        routers.add(v1,v2);
    }
    routers.advancedMultipleDFS();
    

}