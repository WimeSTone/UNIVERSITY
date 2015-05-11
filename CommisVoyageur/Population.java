package example;

import java.util.Vector;
import java.util.Random;

public class Population {

	private Vector <Chromosome> population = new Vector <Chromosome>();

	Population()
	{
		population = new Vector <Chromosome>();
	}


	Population(int inNumberOfChromosomes)
	{
		//System.out.println("Cap ");
		//System.out.println("SetChrom ");
		for(int i = 0; i < inNumberOfChromosomes; i++)
		{
			Chromosome temp = new Chromosome();
			population. insertElementAt(temp, i);
		}
	}



	public Vector <Chromosome> getPopulation() {
		return population;
	}

	public void setPopulation(Vector <Chromosome> population) {
		this.population = population;
	}



	public void crossover()
	{

		Population parents = new Population();
		Random myRand = new Random();
		int crossoverPossibility;


		//parents choice
		for(int i = 0; i < population.size(); i++)
		{
			int iCandidate, jCandidate;
			iCandidate = myRand.nextInt(population.size());
			jCandidate = myRand.nextInt(population.size());
			//System.out.println("capa = " +population.size()+"i = " +iCandidate+ " j = " + jCandidate);
			if(population.get(iCandidate).fitnessFunction() < population.get(jCandidate).fitnessFunction())
			{	try {
				Chromosome temp = new Chromosome();
				temp = population.get(jCandidate).clone();
				temp.mask = (Vector<Integer>) population.get(jCandidate).mask.clone();
				parents.population.add(temp);
			  } catch (CloneNotSupportedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			  }
			}
			
			else
			{
				try {
					Chromosome temp = new Chromosome();
					temp = population.get(jCandidate).clone();
					temp.mask = (Vector<Integer>) population.get(jCandidate).mask.clone();
					parents.population.add(temp);
				} catch (CloneNotSupportedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
		//crossover
		for(int i = 0; i < parents.population.size(); i++)//
		{
			crossoverPossibility = myRand.nextInt(10);
			if( crossoverPossibility < 9)
			{
				int iCandidate, jCandidate;
				iCandidate = myRand.nextInt(parents.population.size());
				jCandidate = myRand.nextInt(parents.population.size());
				odnotoch(parents.population.get(iCandidate), parents.population.get(jCandidate));
			}

		}

		population = (Vector<Chromosome>) parents.population.clone();
		for(int i = 0; i < parents.population.size(); i++)
		{
			try {
				population.set(i, parents.population.get(i).clone());
			} catch (CloneNotSupportedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}


	@SuppressWarnings("unchecked")
	private void odnotoch(Chromosome chromosome1, Chromosome chromosome2)
	{

		Random myRand = new Random();
		int point = myRand.nextInt(chromosome1.mask.size()-1)+1;
		Chromosome temp1 = new Chromosome();
		try {

			temp1 = chromosome1.clone();
			temp1.mask = (Vector<Integer>) chromosome1.mask.clone();
		} catch (CloneNotSupportedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		Chromosome temp2 = new Chromosome();
		try {
			temp2 = chromosome2.clone();
			temp2.mask = (Vector<Integer>) chromosome2.mask.clone();
		} catch (CloneNotSupportedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}


		for(int i = 0; i < point; i++)
		{
			int tempFromChrom =  chromosome2.mask.get(i);
			int tempFromTemp = temp1.mask.get(i);
			chromosome1.mask.set(i, tempFromChrom); 
			chromosome2.mask.set(i, tempFromTemp); 
		}

		if(chromosome1.fitnessFunction() > temp1.fitnessFunction())
		{
			try {
				chromosome1 = temp1.clone();
			} catch (CloneNotSupportedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		if(chromosome2.fitnessFunction() > temp2.fitnessFunction())
		{
			try {
				chromosome2 = temp2.clone();
			} catch (CloneNotSupportedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}

	}


	public boolean isVyrod()
	{
		Chromosome current = new Chromosome();
		current = population.get(0);
		for(int i = 1; i < population.size(); i++)
		{	
			for(int j = 0; j < population.get(i).mask.size(); j++)
			if(	(population.get(i).mask.get(j) != current.mask.get(j)))
				return false;
		}

      //  System.out.println("VYRODZHENA!VYRODZHENA!VYRODZHENA!VYRODZHENA!VYRODZHENA!");
		return true;
	}

	@SuppressWarnings("unchecked")
	private void dvotoch(Chromosome chromosome1, Chromosome chromosome2)
	{
		Random myRand = new Random();
		int point1 = myRand.nextInt(chromosome1.mask.capacity()-1)+1;
		int point2 = myRand.nextInt(chromosome1.mask.capacity()-1)+1;
		int minPoint, maxPoint;
		if(point1 < point2)
		{
			minPoint = point1;
			maxPoint = point2;
		}
		else
		{
			minPoint = point2;
			maxPoint = point1;
		}

		Chromosome temp1 = new Chromosome();
		try {

			temp1 = chromosome1.clone();
			temp1.mask = (Vector<Integer>) chromosome1.mask.clone();
		} catch (CloneNotSupportedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		Chromosome temp2 = new Chromosome();
		try {
			temp2 = chromosome2.clone();
			temp2.mask = (Vector<Integer>) chromosome2.mask.clone();
		} catch (CloneNotSupportedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}


		for(int i = 0; i < minPoint; i++)
		{
			chromosome1.mask.set(i, chromosome2.mask.get(i)); 
			chromosome2.mask.set(i, temp1.mask.get(i)); 
		}
		for(int i = maxPoint; i < chromosome1.mask.size(); i++)
		{
			chromosome1.mask.set(i, chromosome2.mask.get(i)); 
			chromosome2.mask.set(i, temp1.mask.get(i)); 
		}


	}

	public void showPopulation()
	{

		System.out.println("The population is : ");
		for(int i = 0; i < population.size(); i++)
		{
			System.out.println(i+" chromosome");

			population.get(i).showChromosome();
		}

	}

	public Chromosome solution()
	{
		Chromosome currentBestPath = new Chromosome();
		currentBestPath = population.get(0);
		for(int i = 1; i < population.size(); i++)
		{	
			if(	(population.get(i).fitnessFunction() < currentBestPath.fitnessFunction()))
				currentBestPath = population.get(i);
			break;
		}

		return currentBestPath;
	}


	public void mutationHARD()
	{
		Random randomGenerator = new Random();
		//int i = randomGenerator.nextInt(population.get(1).mask.size());
		//System.out.println("In mut HARD!!!" + this.population.size());
		for(int i = 0; i < this.population.size(); i++)
		{
			for(int j = 0; j < this.population.get(i).mask.size(); j++)
			{
				//System.out.println("i = " + i );
				//System.out.println("before");
				//population.get(i).showChromosome();
				population.get(i).mutation(randomGenerator.nextInt(4));
				//System.out.println("after");
				//population.get(i).showChromosome();
				//showPopulation();
			}

		}
	}
}
