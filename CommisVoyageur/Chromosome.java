package example;

import java.util.Vector;
import java.util.Random;


public class Chromosome implements Cloneable{
	
	 public static Vector <City> cities = new Vector<City>();
	 public Vector <Integer> mask= new Vector<Integer>();
	
	 private int numberOfCities;
	
	  public Chromosome clone() throws CloneNotSupportedException {
          return (Chromosome)super.clone();
    }

	  Chromosome()
	  {
		  numberOfCities = cities.size();
		  mask = new Vector<Integer>(numberOfCities);
		  Random randomGenerator = new Random();

		  for(int i = 0; i < numberOfCities; i++)
		  {
			  mask.add(randomGenerator.nextInt(numberOfCities - i));
			//  mask.add(numberOfCities - i-1);
			 // System.out.println("Index " + i +"=" + mask.get(i));
		  }
		
	}

   public Vector<City> decoding() 
   { 
	   Vector <City> cloneCities = new Vector <City>();
	   Vector <City> decodedCities = new Vector <City>();
	   cloneCities = (Vector<City>)cities.clone();
	   for(int i = 0; i < mask.size(); i++)
	   {
		   decodedCities.add(cloneCities.get(mask.get(i)));
		   cloneCities.removeElementAt(mask.get(i));
	   }
	   
	   return decodedCities;
   }

	public double fitnessFunction(){
		
		double length = 0.0;
		for(int i = 0; i < mask.size(); i++){
			if(i == mask.size()-1)
				length = length + Math.sqrt(Math.pow((decoding().get(i).getX() - decoding().get(0).getX()), 2) + Math.pow((decoding().get(i).getY() - decoding().get(0).getY()), 2) );
			else
				length = length + Math.sqrt(Math.pow((decoding().get(i+1).getX() - decoding().get(i).getX()), 2) + Math.pow((decoding().get(i+1).getY() - decoding().get(i).getY()), 2) );

		}
		return length;
	}

	public void mutation(int probability){
		Random randomGenerator = new Random();
		//int probability = randomGenerator.nextInt(10);
		int randPlace = randomGenerator.nextInt(numberOfCities-1);
		int randValue = randomGenerator.nextInt(mask.size()-randPlace);
		if(probability < 3)
		{
			
		//	System.out.println("IN MUTATION"+"randPlace = " + randPlace +" randValue" + randValue);
			this.mask.set(randPlace, randValue);
		}
	}


	public void showChromosome()
	{
		//System.out.println("The chromosome is : ");
		for(int i = 0; i < numberOfCities; i++)
		{
			System.out.print(mask.get(i)+ " ");
		}
		System.out.println("");
	}
	
}