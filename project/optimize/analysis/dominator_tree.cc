#include "dominator_tree.h"
#include "../../include/ir.h"

void DomAnalysis::Execute() {
    for (auto [defI, cfg] : llvmIR->llvm_cfg) {
        DomInfo[cfg].C=cfg;//add 2024.12.18
        DomInfo[cfg].BuildDominatorTree();
    }
}

/*void dfs(int now,std::vector<int>& isvisit,std::vector<int>& ord, std::vector<std::vector<LLVMBlock>>& G){
    isvisit[now]=true;
    for(auto i :G[now])
        if(!isvisit[i->block_id]) dfs(i->block_id,isvisit,ord,G);
    ord.push_back(now);
}*/

void dfs(int now,int del,std::vector<int>& isvisit, std::vector<std::vector<LLVMBlock>>& G){
    //std::cout<<now<<' '<<del<<'\n';
    isvisit[now]=true;
    for(auto i :G[now])
        if(!isvisit[i->block_id] && i->block_id != del) dfs(i->block_id,del,isvisit,G);
}

void DominatorTree::BuildDominatorTree(bool reverse) { 
    //TODO("BuildDominatorTree"); 2024.12.16
    auto &G = C->G;
    auto &invG = C->invG;

    int label = C->label + 1;


    //std::cout<<label;

    idom.clear();//直接支配
    dom_tree.clear();//支配树
    dom_tree.resize(label);

    //std::vector<int> ord;//后序
    //std::vector<int> isvisit;
    isvisit.clear();
    isvisit.resize(label);

    

    //dfs(0,isvisit,ord,G);
    /*std::stack<std::pair<int,bool> > bfs;
    bfs.emplace(0,false);
    isvisit[0]=1;
    while(!bfs.empty()){
        auto now = bfs.top();
        bfs.pop();
        if(now.second)
            ord.push_back(now.first);
        else{
            bfs.emplace(now.first,true);
            for(auto i : G[now.first]){
                if(!isvisit[i->block_id]){
                    isvisit[i->block_id]=1;
                    bfs.emplace(i->block_id,false);
                }
            }
        }
    }*/
    //for(auto i:ord){
    //    std::cout<<i<<' ';
    //}

    isdom.clear();
    isdom.resize(label);
    isdom[0].insert(0);

    dfs(0,-1,isvisit,G);
    std::set<int> useful;
    for(int j=1;j<label;j++){
        if(isvisit[j])
            useful.insert(j);
    }
   
    for(auto i:useful){
        if(i){
            //std::cout<<i;
            isdom[i].insert(0);
            isvisit.clear();
            isvisit.resize(label);
            //for(int k=0;k<label;k++)
            //    isvisit[k]=0; 
            dfs(0,i,isvisit,G);
            for(auto j:useful){
                if(!isvisit[j])
                    isdom[j].insert(i);
            }
        }
    }

    /*bool update;
    do{
        update = false;
        for(int i= ord.size()-1;i>=0;i--){
            int now = ord[i];
            std::set<int> tmp;
            if(invG[now].empty())
                tmp.insert(now);
            else{
                tmp = isdom[(invG[now][0])->block_id];
                for(auto j : invG[now]){
                    std::set<int> tmp1;
                    int id = j->block_id;
                    //if(now==2)std::cout<<id;
                    for(int d : tmp){
                        if(isdom[id].count(d))
                            tmp1.insert(d);
                    }
                    tmp = tmp1;
                }
                tmp.insert(now);
            }
            //std::cout<<invG[2][0]->block_id<<isdom[1].count(1);
            std::cout<<now<<":(";
            for(auto i:isdom[now])std::cout<<i<<' ';
            std::cout<<")(";
            for(auto i:tmp)std::cout<<i<<' ';
            std::cout<<")\n";
            if(isdom[now]!=tmp){
                update = true;
                isdom[now] = tmp;
            }
        }
    }
    while(update);*/

    for(int i = 0; i<isdom.size();i++){
        bool is=false;
        //std::cout<<i<<':';
        for(int j : isdom[i]){
            //std::cout<<j<<' ';
            dom_tree[i].push_back((*C->block_map)[j]);
            if(isdom[j].size() == isdom[i].size() - 1){
                idom.push_back((*C->block_map)[j]);
                //std::cout<<i<<' '<<j<<'\n';
                is = true;
            }
        }
        //std::cout<<'\n';
        if(!is) idom.push_back(nullptr);
    }

    df.clear();
    df.resize(label);

    for (int i = 0; i<isdom.size(); i++) {
        if(invG[i].size()>=2){
            int id = idom[i]->block_id;
            for(auto b : invG[i]){
                int idt = b->block_id; 
                while(id != idt){
                    df[idt].insert(i);
                    idt = idom[idt]->block_id;
                }
            }
        }
    }

}

std::set<int> DominatorTree::GetDF(std::set<int> S) { 
    //TODO("GetDF"); 2024.12.16
    std::set<int> ret;
    for(int i : S)
        ret.insert(df[i].begin(),df[i].end());
    return ret;
}

std::set<int> DominatorTree::GetDF(int id) { 
    //TODO("GetDF"); 2024.12.16
    return df[id];
}

bool DominatorTree::IsDominate(int id1, int id2) { 
    //TODO("IsDominate"); 2024.12.16
    if(isdom[id2].count(id1)>0)
        return true;
    return false;
}
