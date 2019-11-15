#include <iostream>
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
		bool operator == (const Gene &gene) const {
			return ((Trait)==(gene.Trait));}
		bool operator > (const Gene &gene) const {
			return ((Trait)>(gene.Trait));}
		bool operator < (const Gene &gene) const {
			return ((Trait)<(gene.Trait));}
	};
	class Genetic{
		public:
		std::set<Gene> DNA;
		Genetic(Genetic& genetic): DNA(genetic.DNA) {}; //TODO: Init
		Genetic() {};
		Genetic operator + (const Genetic &Mate){
			/*for(Gene gene : this.Dna){
				//TODO: chance to skip
				//TODO: chose gene based on CrossFactor
				//TODO: Mutate gene based on MutationFactor
			}*/
			//TODO: chance to add gene
		}
	};
	class Individual{
		Genetic genetic;
		public:
		Individual(Genetic& genetics): genetic{genetics} {};
		Individual(Individual&& individual): genetic{individual.genetic} {};
		double Perform();
	};
	class ENV {
		std::chrono::high_resolution_clock ST_Chrono;
		std::random_device ST_RD;
		std::mt19937_64 Twist;
		std::uniform_real_distribution<> ST_UNIFORM; 
		public:
		std::vector<Individual> population;
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
			ST_UNIFORM = std::uniform_real_distribution<double> {0,1};
		}*/
		ENV(double MutationFactor=.001,double MutationRange=1,int Population=100,double CrossFactor=.5):
			MF(MutationFactor), MR(MutationRange), PP(Population),CF(CrossFactor) {
			Twist= std::mt19937_64 {}; 
			auto seed = ST_Chrono.now().time_since_epoch().count();
			Twist.seed(seed);
			ST_UNIFORM = std::uniform_real_distribution<double> {0,1};
		}
		double Fitness(Individual ind){
			return ind.Perform();}
		double Postfit(double fitness);
		Genetic Distribute(){

		}
		void Populate() {
			Genetic genetic[PP];
			for(int i=0;i<1;i++){
				for(int j=0;j<PP;j++){
					double random=.5;
					genetic[j].DNA.insert(Gene {i,random});
				}
			}
			for(int i=0;i<PP;i++){
				population.push_back(Individual{genetic[i]});
			}
		}
	};
}
double genetica::Individual::Perform(){
	return (*(this->genetic.DNA.find(Gene(0)))).Value; 
}
int main(){
	using namespace genetica;
	ENV env;
	env.Populate();
	std::cout << env.population[20].Perform();
	std::cout << env.population[70].Perform();
	return 0;
}
