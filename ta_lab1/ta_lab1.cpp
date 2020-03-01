#include <iostream>
#include <fstream>
#include <list>
#include <vector>

//using namespace std;

class Impl
{
public:
	int Num;
	int Ind;
	int P;
	bool Pw;
	bool Indef;

	Impl(int Num=0, int P=0, bool Indef=false)
	{
		this->Num = Num;
		this->Ind = count(Num);
		this->P = P;
		this->Pw = false;
		this->Indef = Indef;
	}

	static int count(int v){
		int c;
		for(c=0; v; c++){
			v &=v-1;
		}
		return c;
	}

	static bool patch(Impl &i, Impl &j, Impl &ij){
		if((i.P == j.P)&&(j.Ind-i.Ind==1)&&(count(j.Num-i.Num)==1)){
			i.Pw=1;
			j.Pw=1;
			ij.Num=i.Num;
			ij.P=i.P+(j.Num-i.Num);
			ij.Ind=count(i.Num);
			ij.Indef=i.Indef&j.Indef;
			return true;
		}
		return false;
	}
};

std::ostream& operator<<(std::ostream &strm, const Impl &impl) {
  return strm << "Num=" << impl.Num << "\tInd=" << impl.Ind << "\tP=" << impl.P << "\tPw=" << impl.Pw << "\tIndef=" << impl.Indef << "\n";
}

void test() {
	std::cout << "Hello!"<< std::endl;
	Impl impl = Impl(7, 0, 1);
	Impl impl1 = Impl(6, 0, 1);
	Impl impl2 = Impl(0, 0, 1);
	Impl impl3 = Impl(1, 0);

	printf("%d - %s - %d\n", impl.Num,impl.Indef?"true":"false", Impl::count(5-6));
	std::cout << impl2;
	std::cout << impl3;
	Impl tmpImpl = Impl();
	if (Impl::patch(impl2, impl3, tmpImpl)) {
		std::cout << tmpImpl;
	} else {
		delete &tmpImpl;
	}
	std::cout << impl2;
	std::cout << impl3;
}

void printArray(const std::vector<Impl> &ImplArray) {
    std::cout << "Num\t";
	for each(Impl n in ImplArray) {
		std::cout << n.Num << "\t";
    }
    std::cout << "\n";
    std::cout << "Ind\t";
	for each(Impl n in ImplArray) {
		std::cout << n.Ind << "\t";
    }
    std::cout << "\n";
    std::cout << "P\t";
	for each(Impl n in ImplArray) {
		std::cout << n.P << "\t";
    }
    std::cout << "\n";
    std::cout << "Pw\t";
	for each(Impl n in ImplArray) {
		std::cout << n.Pw << "\t";
    }
    std::cout << "\n";
    std::cout << "Idf\t";
	for each(Impl n in ImplArray) {
		std::cout << n.Indef << "\t";
    }
    std::cout << "\n";
}

std::vector<Impl> loadArray(std::string fn) {
	int fn_len = fn.size();
    std::vector<Impl> ImplArray;
    for (int i=0; i < fn_len; i++){
    	Impl *tmpImpl;
    	switch(fn[i]) {
    		case '0':
    			break;
    		case '1':
    			tmpImpl = new Impl(i);
    			//std::cout << *tmpImpl;
    			ImplArray.push_back(*tmpImpl);
    			break;
    		case '-':
    			tmpImpl = new Impl(i,0,true);
    			ImplArray.push_back(*tmpImpl);
    			break;
    		default:
    			std::cout << "Wrong sequence symbol:\t"<<fn[i]<<" (only 0's,1's and '-' acceptable)\n";
    			exit(1);

    	}
    }
    return ImplArray;
}

// std::vector<Impl> Skleika(std::vector<Impl> &ImplArray) {
// 	int s = ImplArray.size();
// 	std::vector<Impl> newImplArray;
//     for(int i=0; i<s-1; i++) {
//     	for(int j=i+1; j<s; j++) {
//     		Impl tmpImpl;
//     		//std::cout << ImplArray[i];
//     		if(Impl::patch(ImplArray[i],ImplArray[j],tmpImpl)) {
//     			newImplArray.push_back(tmpImpl);
//     		}
//     	}
//     }
//     return newImplArray;
// }

std::vector<Impl> Skleika(std::vector<Impl> &ImplArray) {
	//int s = ImplArray.size();
	std::vector<Impl> newImplArray;
    for(int i=0; i<ImplArray.size()-1; i++) {
    	for(int j=i+1; j<ImplArray.size(); ) {
    		Impl tmpImpl;
    		//std::cout << ImplArray[i];
    		if((ImplArray[i].Num==ImplArray[j].Num)&&(ImplArray[i].P==ImplArray[j].P)) {
    			ImplArray.erase(ImplArray.begin()+j);
    		} else {
	    		if(Impl::patch(ImplArray[i],ImplArray[j],tmpImpl)) {
	    			newImplArray.push_back(tmpImpl);
    			}
    			j++;
    		}
    	}
    }
    return newImplArray;
}


std::vector<Impl> findTDNF(std::vector<Impl> &ImplArray) {
	std::vector<Impl> tdnf;
	std::vector<Impl> newImplArray;

	std::cout << "-----steps-----" << "\n";

	while(ImplArray.size()>0){
		newImplArray = Skleika(ImplArray);
		printArray(ImplArray);
		for each(Impl tmpImpl in ImplArray) {
			if(!tmpImpl.Pw && !tmpImpl.Indef) {
				// if(tdnf.size()==0 || tmpImpl.P!=tdnf.back().P) {
					tdnf.push_back(tmpImpl);
				// }
			}
		}
		ImplArray = newImplArray;
	}

    return tdnf;
}


bool check(const Impl &src, const Impl &tdnf) {
	// std::cout << (src.Num & ~(uint(tdnf.P))) << " "<< tdnf.Num;
	if((src.Num & ~tdnf.P) == tdnf.Num) {
		return true;
	}
	return false;
}


std::vector<Impl> findMDNF(const std::vector<Impl> &ImplArray, std::vector<Impl> &tdnf) {
	// std::vector<Impl> mdnf;
	std::vector<Impl> newtdnf;

	for(int k=0;k<tdnf.size(); ) {
		newtdnf = tdnf;
		newtdnf.erase(newtdnf.begin()+k);
		// printArray(newtdnf);
		//std::cout << ImplArray[4] << tdnf[1];
		//std::cout << check(ImplArray[4], tdnf[1]);
		bool f;
		for each(Impl i in ImplArray) {
			if(i.Indef==0) {
				f = false;
				for each(Impl t in newtdnf) {
					f = check(i, t); 
					if(f) break;
					//std::cout << check(i, t) << i.Num << "\n";
				}
				if(!f) {
					// std::cout << i.Num << "\n";
					break;
				}
			}
		}
		if(!f) {
			k++;
		} else {
			tdnf = newtdnf;
			k=1;
		}
	}
	//std::cout << f << "\n";
	
    return tdnf;
}

std::string Impl2string(const Impl &i, int N) {
	std::string out = "";
	int r=0;
	while (N >>= 1) r++;
	r++;
	N = 1<<r;
	if(i.P == (N-1)) out += "1";
	else {
		// std::cout << N <<' ' << (1<<r);
		int Num=i.Num; 
		int P=i.P;
		for(int k=r-1; k>=0; k--) {
		
			// std::cout << k << " ";
			// std::cout << Num << " ";
			// std::cout << P << " \n";
			// // if (!(P&1)) {
			// 	if (Num&1) std::cout << "x" << k;
			// 	else std::cout << "!x" << k;
			// }
			if (!(P&(1<<k))) {
				if (Num&(1<<k)) out += "1";
				else out += "0";
			}
			else out += "-";

			// Num>>=1;
			// P>>=1;
			
		}
	}
	return out;
}

std::string ImplArray2string(const std::vector<Impl> &ImplArray, int N) {
	std::string out = "";
	if(ImplArray.size()>0) {
		for each(Impl i in ImplArray) {
			out += "\n";
			out += Impl2string(i, N-1);
		}
	} else {
		out += "";
	}
	// out += "\n";
	return out;
}

int main(int argc, char* argv[])
{
	//test();
	
	std::string fn = "";
	//if(argc==2) {
		fn = argv[1];
	   	std::ifstream file("scale.txt");
	    if (!file) {
		std::cout << "Error reading file. No such file!\n";
		exit(1);
	    }
		fn ="";
		char cur_char;
		while(1) {
			file.get(cur_char);
			if (file.eof()) break;
			fn += cur_char;
		} 
		file.close();
	//}// else {
    	//fn = "1111101-11-01110";//"1000100010101011";//"1111101-11-10010";
    }
	//std::cout << fn.c_str();
    //открываем файловый поток связанный с входным файлом
    std::vector<Impl> tdnf;
    std::vector<Impl> mdnf;
	std::vector<Impl> ImplArray = loadArray(fn);
	std::cout << "-----SDNF-----" << "\n";
    printArray(ImplArray);
    tdnf = findTDNF(ImplArray);
	std::cout << "-----TDNF-----" << "\n";
	printArray(tdnf);
	ImplArray = loadArray(fn);
    mdnf = findMDNF(ImplArray, tdnf);
	std::cout << "-----MDNF-----" << "\n";
	printArray(mdnf);
	// std::cout << "-----F(x_i)-----" << "\n";
	std::ofstream file("mdnf.txt");
	file << "xyzu";
	std::string s = ImplArray2string(mdnf, fn.size());
	for each(char ch in s) file << ch;
    file.close();
	return 0;
}

