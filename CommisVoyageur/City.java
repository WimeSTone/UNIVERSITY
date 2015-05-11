package example;

public class City {

	private int xCoordinate;
	private int yCoordinate;

	public int getX()
	{
		return xCoordinate;
	}

	public int getY()
	{
		return yCoordinate;
	}

	public void setCoordinate(int inX, int inY){
		xCoordinate = inX;
		yCoordinate = inY;
	}

	public void setX(int inX)
	{
		yCoordinate = inX;
	}

	public void setY(int inY)
	{
		yCoordinate = inY;
	}

	public void showCoordinate(){
		System.out.println("X = " + xCoordinate + "; Y = " + yCoordinate + ";");
	}

	City(int inX, int inY)
	{
		xCoordinate = inX;
		yCoordinate = inY;
	}
	City(){}

}


