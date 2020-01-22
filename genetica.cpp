#include <iostream>
#include <math.h>
#include <string>
#include <algorithm>
#include <set>
#include <random>
#include <chrono>
namespace genetica {
	typedef double TGene; //TODO: template
	struct Gene {
		int Trait;
		TGene Value;
		Gene(int trait, TGene value): Trait(trait),Value(value) {};
		Gene(int trait): Trait(trait) {};
		//Gene(Gene&& gene): Trait(gene.Trait),Value(gene.Value) {};
		//Gene(Gene& gene): Trait(gene.Trait),Value(gene.Value) {};
		bool operator == (const Gene &gene) const {
			return ((Trait)==(gene.Trait));}
		bool operator > (const Gene &gene) const {
			return ((Trait)>(gene.Trait));}
		bool operator < (const Gene &gene) const {
			return ((Trait)<(gene.Trait));}
	};
	class Individual{
		public:
		std::set<Gene> DNA;//TODO: Private
		Individual(std::set<Gene>& genetics): DNA{genetics} {};
		Individual(Individual&& individual): DNA{individual.DNA} {};
		double Perform();
		std::string Checkout();
	};
	class ENV {
		std::chrono::high_resolution_clock ST_Chrono;
		std::random_device ST_RD;
		std::mt19937_64 Twist;
		std::uniform_real_distribution<> ST_UNIFORM_REAL; 
		std::uniform_int_distribution<> ST_UNIFORM_INT; 
		public:
		typedef std::vector<Individual> Population;
		typedef std::vector<double> ScoreBoard;
		typedef std::vector<int> Selection; 
 		//Individual *population;
		double MF,MR,CF;
		int PP;
		/*static void ST_Init(){
			ENV::MutationFactor=.001;
			ENV::MutationRange=1;
			ENV::Population=100;
			ENV::CrossFactor=.5;
			Twist= std::mt19937_64 {}; 
			auto seed = ST_Chrono.now().time_since_epoch().count();
			Twist.seed(seed);
			ST_UNIFORM_REAL = std::uniform_real_distribution<double> {0,1};
		}*/
		ENV(double MutationFactor=.01,double MutationRange=1,int Population=100,double CrossFactor=.5):
			MF(MutationFactor), MR(MutationRange), PP(Population),CF(CrossFactor) {
			Twist= std::mt19937_64 {}; 
			auto seed = ST_Chrono.now().time_since_epoch().count();
			Twist.seed(seed);
			ST_UNIFORM_REAL = std::uniform_real_distribution<double> {0,1};
		}
		double Fitness(Individual ind){
			return ind.Perform();}
		double Postfit(double fitness);

		Population Populate() {
			std::set<Gene> genetics[PP];
			Population population; 
			for(int i=0;i<12;i++){ //HARD
				for(int j=0;j<PP;j++){
					double random=ST_UNIFORM_REAL(ST_RD);
					genetics[j].insert(Gene {i,random});
				}
			}
			for(int i=0;i<PP;i++)
				population.push_back(Individual(genetics[i]));
			return population;
		}
		Selection Select(ScoreBoard scoreboard, int num){
			ST_UNIFORM_REAL.reset();
			ST_UNIFORM_INT= std::uniform_int_distribution<int> {0,scoreboard.size()-1};
			ST_UNIFORM_INT.reset();
			Selection select;
			for(int i=0;i<num;i++){
				double Desire=ST_UNIFORM_REAL(ST_RD);
				for(int j=0;j<PP;j++){
					double score=scoreboard[j];
					if((Desire<=score)&&(std::find(select.begin(),select.end(),j)==select.end())){
						select.push_back(j);
						break;
					}else
						Desire-=score;
				}
				select.push_back(ST_UNIFORM_INT(ST_RD));
selection_found:
				continue;
				
			}
			return select;
		}
		ScoreBoard Evaluate(Population& population){
			ScoreBoard scoreboard;
			double sum=0;
			for(int i=0;i<PP;i++){
				double score = Postfit(population[i].Perform());
				scoreboard.push_back(score);
				sum+=score;
			}
			if(sum)
				for(int i=0;i<PP;i++)
					scoreboard[i]/=sum;
			return scoreboard;
		}
		int Best(ScoreBoard& scoreboard){
			int best=0;
			for(int i=0;i<scoreboard.size();i++)
				if(scoreboard[i]>=best)best=i;
			return best;
		}
		Individual Breed(Population& population,Selection& parents){
			ST_UNIFORM_INT= std::uniform_int_distribution<int> {0,parents.size()-1};
			ST_UNIFORM_INT.reset();
			std::set<Gene> dna;
			for(int i=0;i<12;i++){ //HARD
				//TODO: Chance to skip
				int pick=ST_UNIFORM_INT(ST_RD); //TODO: Controlled Distribution
				double gene = (*(population[parents[pick]].DNA.find(Gene(i)))).Value;
				if(ST_UNIFORM_REAL(ST_RD)<=MF){
					double mutation = (ST_UNIFORM_REAL(ST_RD)*MR)-(MR/2);
					gene+=mutation;
					if(gene<0)gene=0;
					else if(gene>1) gene=1;
				}
				if(gene==0)gene=0.01;
				dna.insert(Gene{i,gene});
				//TODO: Chance to add gene
			}
			return Individual{dna};
		}
		
	};
}
double genetica::Individual::Perform(){
	const char target[]="Hello World!";
	double fitness=0;
	for(int i=0;i<12;i++)
		if((char)(32+((*(this->DNA.find(Gene(i)))).Value*94))==target[i])fitness++; 
	return fitness;
}
std::string genetica::Individual::Checkout(){
	std::string str;
	for(int i=0;i<12;i++)
		str+=(char)(32+((*(this->DNA.find(Gene(i)))).Value*94));
	return str;
}
double genetica::ENV::Postfit(double fitness){
	return exp(5*fitness);
	//return fitness;
}
int main(){
	using namespace genetica;
	ENV env {0.05,2,1000};
	ENV::Population population[2];
	int pop1=0,pop2=1;
	population[pop1]= env.Populate();
	double best=0;
	while(best!=12){
		ENV::ScoreBoard scoreboard = env.Evaluate(population[pop1]);
		best=population[pop1][env.Best(scoreboard)].Perform();
		std::cout << best << "\t";
		std::cout << population[pop1][env.Best(scoreboard)].Checkout() << "\n";
		for(int i=0;i<env.PP;i++){
			ENV::Selection select = env.Select(scoreboard,2);
			population[pop2].push_back(env.Breed(population[pop1],select));
		}
		population[pop1].clear();
		int t=pop1;
		pop1=pop2;
		pop2=t;
		//std::cin.ignore(1);
	}
	return 0;
}
