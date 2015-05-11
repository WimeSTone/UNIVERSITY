package example;

import java.util.Random;
import java.util.Vector;
import java.awt.BasicStroke;
import java.awt.Button;
import java.awt.Canvas;
import java.awt.Color;
import java.awt.Container;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.Label;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.Shape;
import java.awt.TextComponent;
import java.awt.TextField;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.geom.Line2D;
import java.awt.image.ImageObserver;


public  class myGUI extends javax.swing.JFrame implements MouseListener, MouseMotionListener 
{

	Vector<City> inCities1 = new Vector <City> ();
	Graphics gr; 
	Label text;

	public static void main(String[] args) 
	{
		System.out.println("fgd");
		myGUI a = new myGUI("     ");
	}

	public myGUI(String str)
	{
		super (str);
		Container c = getContentPane();
		c.setLayout(new FlowLayout());

		text = new Label("   Text                        ");
		c.add(text);
		Color color = new Color(-1);
		c.setBackground(color);
		addMouseListener(this);
		addMouseMotionListener(this);
		setSize (600, 400);
		setVisible(true);

		Button numOfCItiesButton = new Button("Find the path!");
		c.add(numOfCItiesButton);

		numOfCItiesButton.addActionListener(new ActionListener() {
			@SuppressWarnings("unchecked")
			public void actionPerformed(ActionEvent e) 
			{

				Chromosome.cities = inCities1;
				text.setText("Searching for a path....");
				Random myRand = new Random();
				Population newPopulation = new Population(25);  // инициализация всех нач. условий
				int mutationChell = 0;
				boolean flag = true;

				Chromosome currentSolution = new Chromosome();
				Random randomGenerator = new Random();
				currentSolution = newPopulation.solution();
				int badCount = 0;         // хорошие/плохие решения
				int goodCount = 0;

				while(flag)
				{
					newPopulation.crossover();

					mutationChell = myRand.nextInt(newPopulation.getPopulation().size());
					newPopulation.getPopulation().get(mutationChell).mutation(randomGenerator.nextInt(10));

					if( newPopulation.solution().fitnessFunction() > currentSolution.fitnessFunction())
					{
						goodCount++;
						if(goodCount==Chromosome.cities.size()*600)
							flag = false;
						badCount++;
						if(badCount==3)       // если количество неблагоприятных исходов дошло до 3 - производим произвольные мутации
						{
							newPopulation.mutationHARD();
							badCount = 0;

						}
					}


					if(newPopulation.solution().fitnessFunction() < currentSolution.fitnessFunction())
					{

						try {
							currentSolution = newPopulation.solution().clone();
							currentSolution.mask = (Vector<Integer>) newPopulation.solution().mask.clone();
							goodCount=0;
						} catch (CloneNotSupportedException e1) {
							// TODO Auto-generated catch block
							e1.printStackTrace();
						}

					}

					if(newPopulation.solution().fitnessFunction() == currentSolution.fitnessFunction())
					{
						goodCount++;
						if(goodCount==Chromosome.cities.size()*600)
							flag = false;

					}




				}

				gr = getGraphics(); 
				for(int i = 0; i < currentSolution.decoding().size(); i++)
				{
					if(i == currentSolution.decoding().size()-1)
						gr.drawLine( currentSolution.decoding().get(i).getX(), currentSolution.decoding().get(i).getY(),currentSolution.decoding().get(0).getX(),currentSolution.decoding().get(0).getY()); 
					else
						gr.drawLine(currentSolution.decoding().get(i).getX(), currentSolution.decoding().get(i).getY(),currentSolution.decoding().get(i+1).getX(),currentSolution.decoding().get(i+1).getY()); //рисуем линию от точки (10,10)
				}
			}

		});


	}

	public void mousePressed (MouseEvent e)
	{

		gr = getGraphics(); 
		inCities1.add(new City(e.getX(),e.getY())); 
		//gr.drawLine(e.getX(), e.getY(),e.getX(), e.getY());
		 Graphics2D g2 = (Graphics2D) gr;
         g2.setStroke(new BasicStroke(5));
         g2.draw(new Line2D.Float(e.getX(), e.getY(),e.getX(), e.getY()));
	}



	public void mouseExited(MouseEvent e) {

	}


}







