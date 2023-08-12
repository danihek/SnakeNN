	//#include "NeuralNetworkClass.h"
	#include <vector>
	#include <iostream>
	#include <cstdlib>
	#include <cassert>
	#include <cmath>
	#include <ctime>
	#include <random>
	#include <fstream>
	#include <string>
	#include <sstream>
	//#include <windows.data.json.h>

	class RRRRandom {
	public:
		static RRRRandom& get();

		RRRRandom(const RRRRandom& other) = delete;
		RRRRandom& operator=(const RRRRandom& other) = delete;

		int intInRange(int low, int high);
		float floatInRange(float low, float high);

		template <typename T>
		T getNInRange(T low, T high)
		{
			std::uniform_real_distribution<T> dist(low, high);
			return dist(m_rng);
		}

	private:
		RRRRandom();

		std::mt19937 m_rng;
	};

	RRRRandom& RRRRandom::get()
	{
		static RRRRandom r;
		return r;
	}

	RRRRandom::RRRRandom()
	{
		m_rng.seed(std::time(nullptr));
	}

	int RRRRandom::intInRange(int low, int high)
	{
		std::uniform_int_distribution<int> dist(low, high);
		return dist(m_rng);
	}

	float RRRRandom::floatInRange(float low, float high)
	{
		std::uniform_real_distribution<float> dist(low, high);
		return dist(m_rng);
	}

	struct Connection
	{
		double weight;
		double deltaWeight;
	};

	class Neuron;

	typedef std::vector<Neuron> Layer;

	class Neuron
	{
	public:
		Neuron(unsigned numOutputs, unsigned myIndex);

		void setOutputVal(double val) { m_outputVal = val; }
		double getOutputVal(void) const { return m_outputVal; }
		void feedForward(const Layer& prevLayer);
		void calcOutputGradients(double targetVal);
		void calcHiddenGradients(const Layer& nextLayer);
		void updateInputWeights(Layer& prevLayer);
		void SetWeights(Neuron* nn);
		void MutateLayer(float mutationChance, float mutationAmount);
		void mutateW(float chance, float mag);
		double getWeightByID(int id) { return m_outputWeights.at(id).weight; }
		void setWeightByID(int id, double _weight) { m_outputWeights.at(id).weight = _weight; }
		int weightSize() { return m_outputWeights.size(); }

	private:
		static double eta;
		static double alpha;
		static double activationFunction(double x);
		static double activationFunctionDerivative(double x);
		static double randomWeight(void) { return RRRRandom::get().floatInRange(0.0f, 1.0f); }
		double sumDOW(const Layer& nextLayer) const;
		double m_outputVal;
		unsigned m_myIndex;
		double m_gradient;
		std::vector<Connection> m_outputWeights;
	};

	double Neuron::eta = 0.15;

	double Neuron::alpha = 0.5;

	double Neuron::activationFunction(double x)
	{
		//return log(1.0 + std::exp(x));
		//return (1.0f / 1.0f + std::exp(-x));
		return std::tanh(x);
	}

	double Neuron::activationFunctionDerivative(double x)
	{
		return 1.0 - x * x;
	}

	double Neuron::sumDOW(const Layer& nextLayer) const
	{
		double sum = 0.0;

		for (unsigned n = 0; n < nextLayer.size() - 1; n++)
		{
			sum += m_outputWeights[n].weight * nextLayer[n].m_gradient;
		}

		return sum;
	}

	Neuron::Neuron(unsigned numOutputs, unsigned myIndex)
	{
		for (unsigned c = 0; c < numOutputs; c++)
		{
			m_outputWeights.push_back(Connection());
			m_outputWeights.back().weight = randomWeight();
		}

		m_myIndex = myIndex;
	}

	void Neuron::feedForward(const Layer& prevLayer)
	{
		double sum = 0.0;

		for (unsigned n = 0; n < prevLayer.size(); n++)
		{
			sum += prevLayer.at(n).getOutputVal() * prevLayer.at(n).m_outputWeights[m_myIndex].weight;
		}

		m_outputVal = Neuron::activationFunction(sum);
	}

	void Neuron::calcOutputGradients(double targetVal)
	{
		double delta = targetVal - m_outputVal;
		m_gradient = delta * Neuron::activationFunctionDerivative(m_outputVal);
	}

	void Neuron::calcHiddenGradients(const Layer& nextLayer)
	{
		double dow = sumDOW(nextLayer);
		m_gradient = dow * Neuron::activationFunctionDerivative(m_outputVal);
	}

	void Neuron::updateInputWeights(Layer& prevLayer)
	{
		for (unsigned i = 0; i < prevLayer.size(); i++)
		{
			Neuron& neuron = prevLayer[i];
			double oldDeltaWeight = neuron.m_outputWeights[m_myIndex].deltaWeight;

			double newDeltaWeight =
				eta
				* neuron.getOutputVal()
				* m_gradient
				+ alpha
				* oldDeltaWeight;

			neuron.m_outputWeights[m_myIndex].deltaWeight = newDeltaWeight;

		}
	}

	void Neuron::SetWeights(Neuron* nn)
	{
		m_outputWeights.clear();
		for (unsigned c = 0; c < nn->m_outputWeights.size(); c++)
		{
			m_outputWeights.push_back(Connection());
			m_outputWeights.back().weight = nn->m_outputWeights.at(c).weight;
		}
	}

	void Neuron::MutateLayer(float mutationChance, float mutationAmount)
	{
		for (int i = 0; i < m_outputWeights.size(); i++)
		{
			m_outputWeights.at(i).weight += (RRRRandom::get().floatInRange(-1.0f, 1.0f) * mutationAmount);
		}
	}

	void Neuron::mutateW(float chance, float mag)
	{
		for (int i = 0; i < m_outputWeights.size(); i++)
		{
			double fac = ((static_cast<double>(rand()) / static_cast<double>(RAND_MAX)) * 2 - 1) * mag;
			if (m_outputWeights.at(i).weight + fac > 1)
			{
				m_outputWeights.at(i).weight = 1;
			}
			else if (m_outputWeights.at(i).weight + fac < -1)
			{
				m_outputWeights.at(i).weight = -1;
			}
			else
			{
				m_outputWeights.at(i).weight += fac;
			}
		}
	}

	class Net
	{
	public:


		Net(const std::vector<unsigned>& topology);

		void feedForward(const std::vector<double>& inputVals);
		void backProp(const std::vector<double>& targetVals);
		void getResult(std::vector<double>& resultVals) const;
		void mutate(float chance, float mag);
		void MutateNetwork(float mutationChance, float mutationAmount);
		// , float mag);
		void setUp(Net* net);
		//void readNet(std::string filename);
		void saveNet(std::string filename);
		void draw();
		Net MixNetworks(Net _net);

		Net MixNetworksDeeply(Net _net);

		std::vector<Layer> m_layers; // m_layers[ layerID ] [ neuron ]
	private:
		double m_error;
		double m_recentAvrgError;
		double m_recentAvrgSmoothingError;
		std::vector<unsigned> topol;
	};

	std::string get_timestamp()
	{
		return std::to_string(rand() % 10000)+"Snake";
	}

	void Net::setUp(Net* net)
	{
		for (unsigned i = 0; i < m_layers.size(); i++)
		{
			for (unsigned j = 0; j < m_layers.at(i).size(); j++)
			{
				m_layers.at(i).at(j).SetWeights(&net->m_layers.at(i).at(j));
			}
		}
	}

	Net Net::MixNetworks(Net _net)
	{
		Net n(topol);

		for (int i = 0; i < n.m_layers.size(); i++)
		{
			for (int j = 0; j < m_layers.at(i).size(); j++)
			{
				if (j % 2 == 0)
				{
					n.m_layers.at(i).at(j).SetWeights(&m_layers.at(i).at(j));
				}
				else
				{
					n.m_layers.at(i).at(j).SetWeights(&_net.m_layers.at(i).at(j));
				}
			}
		}

		return n;
	}

	Net Net::MixNetworksDeeply(Net _net)
	{
		Net n(topol);
		int chance = 40;

		for (int i = 0; i < n.m_layers.size(); i++)
		{
			for (int j = 0; j < m_layers.at(i).size(); j++)
			{
				for (int w = 0; w < m_layers.at(i).at(j).weightSize(); w++)
				{
					if (RRRRandom::get().intInRange(0,100) > chance)
					{
						n.m_layers.at(i).at(j).setWeightByID(w, m_layers.at(i).at(j).getWeightByID(w));
					}
					else
					{
						n.m_layers.at(i).at(j).setWeightByID(w, _net.m_layers.at(i).at(j).getWeightByID(w));
					}
				}
			}
		}

		return n;
	}

	void Net::draw()
	{
		int counter = 0;
		for (unsigned i = 0; i < m_layers.size(); i++)
		{
			std::cout << i<<"\n";
			for (unsigned j = 0; j < m_layers.at(i).size(); j++)
			{
				std::cout <<"\t" << j << "\n";
				for (unsigned w = 0; w < m_layers.at(i).at(j).weightSize(); w++)
				{
					std::cout << "\t" << "\t" <<m_layers.at(i).at(j).getWeightByID(w) << "\n";
					counter++;
				}
			}
		}

		std::cout << "\n\nTOTAL CONNECTIONS: " << counter << "\n";
	}

	void Net::mutate(float chance, float mag)
	{
		for (unsigned i = 0; i < m_layers.size(); i++)
		{
			for (unsigned j = 0; j < m_layers.at(i).size(); j++)
			{
				m_layers.at(i).at(j).mutateW(chance, mag);
			}
		}
	}
	void Net::MutateNetwork(float mutationChance, float mutationAmount)
	{
		for (int i = 0; i < m_layers.size(); i++)
		{
			for (int j = 0; j < m_layers.at(i).size(); j++)
			{
				m_layers.at(i).at(j).MutateLayer(mutationChance, mutationAmount);
				//m_layers.at(i).at(j).mutateW(mutationChance, mutationAmount);
			}
			//layers[i]->MutateLayer(mutationChance, mutationAmount);
		}
	}


	void Net::feedForward(const std::vector<double>& inputVals)
	{
		assert(inputVals.size() == m_layers[0].size() - 1); // ERROR

		for (unsigned i = 0; i < inputVals.size(); i++)
		{
			m_layers[0][i].setOutputVal(inputVals[i]);
		}

		// forward propagate

		for (unsigned layerNum = 1; layerNum < m_layers.size(); layerNum++)
		{
			Layer& prevLayer = m_layers[layerNum - 1];
			for (unsigned n = 0; n < m_layers[layerNum].size() - 1; n++)
			{
				m_layers[layerNum][n].feedForward(prevLayer);
			}
		}
	}

	void Net::backProp(const std::vector<double>& targetVals)
	{
		Layer& outputLayer = m_layers.back();
		m_error = 0.0;

		for (unsigned n = 0; n < outputLayer.size() - 1; n++)
		{
			double delta = targetVals[n] - outputLayer[n].getOutputVal();
			m_error += delta * delta;
		}
		m_error /= outputLayer.size() - 1;
		m_error = sqrt(m_error);

		m_recentAvrgError = (m_recentAvrgError * m_recentAvrgSmoothingError + m_error) / (m_recentAvrgSmoothingError + 1.0);

		for (unsigned n = 0; n < outputLayer.size() - 1; n++)
		{
			outputLayer[n].calcOutputGradients(targetVals[n]);
		}

		for (unsigned layerNum = m_layers.size() - 2; layerNum > 0; layerNum--)
		{
			Layer& hiddenLayer = m_layers[layerNum];
			Layer& nextLayer = m_layers[layerNum + 1];

			for (unsigned n = 0; n < hiddenLayer.size(); n++)
			{
				hiddenLayer[n].calcHiddenGradients(nextLayer);
			}
		}

		for (unsigned layerNum = m_layers.size() - 1; layerNum > 0; layerNum--)
		{
			Layer& layer = m_layers[layerNum];
			Layer& prevLayer = m_layers[layerNum - 1];

			for (unsigned n = 0; n < layer.size(); n++)
			{
				layer[n].updateInputWeights(prevLayer);
			}
		}
	}

	void Net::getResult(std::vector<double>& resultVals) const
	{
		resultVals.clear();

		for (unsigned i = 0; i < m_layers.back().size() - 1; i++)
		{
			resultVals.push_back(m_layers.back()[i].getOutputVal());
			//std::cout << m_layers.back()[i].getOutputVal() << ", ";
		}
		//std::cout << std::endl;
	}


	Net::Net(const std::vector<unsigned>& topology)
	{
		topol = topology;
		unsigned numLayers = topology.size();
		for (unsigned layerNum = 0; layerNum < numLayers; layerNum++)
		{
			m_layers.push_back(Layer());
			unsigned numOutputs = layerNum == topology.size() - 1 ? 0 : topology[layerNum + 1];

			for (unsigned neuronNum = 0; neuronNum <= topology[layerNum]; neuronNum++)
			{
				m_layers.back().push_back(Neuron(numOutputs, neuronNum));

				//std::cout << "Made a Neuron!" << std::endl;
			}
			//m_layers.back().back().setOutputVal(RRRRandom::get().floatInRange(-1.f,1.f)); // 1.0 originally
			m_layers.back().back().setOutputVal(1.f); // 1.0 originally
		}
	}

	void Net::saveNet(std::string filename)
	{
		std::ofstream file("models/"+filename, std::ios::app);
		if (!file)
		{
			std::cerr<< "model directory error!"<<std::endl;
			system("mkdir models");
		}
		file.close();
		file.open("models/" + filename, std::ios::app);

		for (int i = 0; i < m_layers.size(); i++)
		{
			for (int j = 0; j < m_layers.at(i).size(); j++)
			{
				for (int w = 0; w < m_layers.at(i).at(j).weightSize(); w++)
				{
					file << std::to_string(m_layers.at(i).at(j).getWeightByID(w))<<'\n';
				}
			}
		}


		file.close();
	}


	Net readNet(std::string filename, std::vector<unsigned> topl)
	{
		Net n_(topl);
		std::ifstream file;
		std::string line;
	

		n_.draw();
		file.open("models/"+filename, std::ios::in);

		if (!file)
		{
			std::cerr << "failed to open file: " << filename << std::endl;
		}
		else 
		{
			int i = 0;
			int j = 0;
			int w = 0;
			int counter = 0;

			while (std::getline(file, line))
			{
				if (w >= n_.m_layers.at(i).at(j).weightSize())
				{
					w = 0;
					j++;
				}
				if (j >= n_.m_layers.at(i).size())
				{
					i++;
					j = 0;
				}
				double temp = std::stod(line);
				std::cout << temp <<"\n";
				n_.m_layers.at(i).at(j).setWeightByID(w, temp);
				counter++;
				w++;
			}
			file.close();

			std::cout << "\n\nTOTAL CONNECTIONS: " << counter << "\n";
		}
		file.close();

		return n_;
	}
