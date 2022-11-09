#include<bits/stdc++.h>

using namespace std;

int dx[4] = {1,0,-1,0};
int dy[4] = {0,1,0,-1};

struct NodeMap{
	int size;
	int width;
	int height;
	vector<pair<int,int> > s;
	
	void init(int width,int height){
		size = 1;
		this->width = width;
		this->height = height;
		while(size < width*height) size *= 2;
		s.resize(2 * size);
	}
	
	void build(vector<pair<int,int> > &v, int x, int lx, int rx){
		if(rx - lx == 1){
			if(lx < v.size()){
				s[x] = v[lx];
			}else {
				s[x] = {-1,-1};
			}
			return;
		}
		int m = (lx + rx) /2;
		build(v,(x<<1)+1, lx, m);
		build(v,(x<<1)+2, m, rx);
		pair<int,int> p1 = s[(x<<1)+1],p2 = s[(x<<1)+2];
		s[x] = {((p1.first==-1&&p1.second==-1)? -1:1),((p2.first==-1&&p2.second==-1)? -1:1)};
	}
	
	void build(){
		vector<pair<int,int> > v;
		for(int i=0;i<height;i++){
			for(int j=0;j<width;j++){
				v.push_back({i,j});
			}
		}
		build(v,0,0,size);	
	}
	
	pair<int,int> getNextNode(int x, int lx, int rx){
		if(rx - lx == 1){
			pair<int,int> p = s[x];
			s[x] = {-1,-1};
			//cout << p.first << " " << p.second << '\n';
			return p;
		} 
		int m = (rx + lx) /2;
		while(true){
			int r = rand()%rx;
			if(r<m&&s[x].first!=-1){
				getNextNode((x<<1)+1,lx,m);
				break;
			}
			if(r>=m&&s[x].second!=-1){
				getNextNode((x<<1)+2,m,rx);
				break;
			}
		}
		pair<int,int> p1 = s[(x<<1)+1],p2 = s[(x<<1)+2];
		s[x] = {((p1.first==-1&&p1.second==-1)? -1:1),((p2.first==-1&&p2.second==-1)? -1:1)};
	}
	
	pair<int,int> getNextNode(){
		return getNextNode(0,0,size);
	}
	
	void print(){
		while(s[0].first!=-1||s[0].second!=-1){
			getNextNode();
		}
	}
};


class cell{
	private:
		bool end,left,bot;
	public:
		cell(){
			end = false;
			left = true;
			bot = true;
		}
		void removeLeft(){
			left = false;
		}
		void isEnd(){
			end = true;
		}
		void removeBot(){
			bot = false;
		}
		bool getEnd(){
			return end;
		}
		bool getBot(){
			return bot;
		}
		bool getLeft(){
			return left;
		}
};

class cellPrinter{
	protected: 
		string botWall = "_";
		string leftWall = "|";
		string rightWall = "|";
		static cellPrinter* instance;
	
	public:
		static cellPrinter getInstance();
		void print(cell c){
			cout <<(c.getLeft()? leftWall:"_") << (c.getBot()? botWall:" ");
			if(c.getEnd()) cout << rightWall << '\n';
			else cout <<"";
		}
		void setRightWall(string rWall){
			rightWall = rWall;
		}
		void setleftWall(string lWall){
			leftWall = lWall;
		}
		void setBotWall(string bWall){
			botWall = bWall;
		}
};

cellPrinter* cellPrinter::instance = nullptr;

cellPrinter cellPrinter::getInstance(){
	if(instance==nullptr){
		instance = new cellPrinter();
	}
	return *instance;
}

class maze{
	private:
		vector<vector<cell> > m;
		int height,width;
		cellPrinter cp;
	public:
		maze(int height,int width){
			this->height = height;
			this->width = width;
			m.resize(height, vector<cell>(width));
			for(int i=0;i<height;i++){
				m[i][width-1].isEnd();
			}
			m[height-1][width-1].removeBot();
			DFS();
		}
		
		void print(){
			for(int i=0;i<width;i++){
				cout << (i==0? "  ":"__") << (i==width-1? "\n":"");
			}
			for(int i=0;i<height;i++){
				for(int j=0;j<width;j++){
					cp.getInstance().print(m[i][j]);
				}
			}
		}
		
		void DFS(){
			vector<vector<bool> > visited(height,vector<bool>(width,false));
			/*queue<pair<int,int> > q;
			NodeMap nmap;
			nmap.init(width,height);
			nmap.build();
			q.push(nmap.getNextNode());
			while(!q.empty()){
				pair<int,int> p = q.front();
				q.pop();
				DFS(p,visited);
			}*/
			DFS({0,0},visited);
		}
		
		void DFS(pair<int,int> node,vector<vector<bool > > &visited){
			//cout << node.first << " " << node.second << '\n';
			system("CLS");
			print();
			visited[node.first][node.second] = true;
			pair<int,int> nextNode = getNeighbour(node,visited);
			while(nextNode.first != -1 && nextNode.second != -1){
				breakWall(node,nextNode);
				DFS(nextNode,visited);
				nextNode = getNeighbour(node,visited);
			}
		}
		
		pair<int,int> getNeighbour(pair<int,int> node, vector<vector<bool> > visited){
			vector<pair<int,int> > an;
			for(int i=0;i<4;i++){
				if(inside(node.first+dx[i],node.second+dy[i])&&!visited[node.first+dx[i]][node.second+dy[i]]){
					an.push_back({node.first+dx[i],node.second+dy[i]});
				}
			}
			if(an.size()==0){
				return {-1,-1};
			}
			int r = rand()%an.size();
			return an[r];
		}
		bool inside(int x,int y){
			return (x>=0&&y>=0&&x<height&&y<width);
		}
		void breakWall(pair<int,int> node, pair<int,int> adj){
			if(node.second+1==adj.second){
				m[adj.first][adj.second].removeLeft();
			}
			if(node.second-1==adj.second){
				m[node.first][node.second].removeLeft();
			}
			if(node.first+1==adj.first){
				m[node.first][node.second].removeBot();
			}
			if(node.first-1==adj.first){
				m[adj.first][adj.second].removeBot();
			}
		}
};

int main(){
	srand (time(NULL));
	int n,m;
	cout << "Enter Height:\n";
	cin >> n;
	cout << "Enter Width:\n";
	cin >> m;
	maze ma(n,m);
	//ma.print();
}
