using System;
using System.Collections;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab5extra
{
    // cordinates for Knight Explorer
    // x - [A, H], A is 1, y - [1, 8]
    class ChessCordinates
    {
        private int _x;
        private int _y;

        // converts (x, y) cords to A-H 1-8 cords
        override public string ToString()
        {
            string resultString;

            switch(this._x)
            {
                case 1:
                    resultString = 'A' + this._y.ToString();
                    break;
                case 2:
                    resultString = 'B' + this._y.ToString();
                    break;
                case 3:
                    resultString = 'C' + this._y.ToString();
                    break;
                case 4:
                    resultString = 'D' + this._y.ToString();
                    break;
                case 5:
                    resultString = 'E' + this._y.ToString();
                    break;
                case 6:
                    resultString = 'F' + this._y.ToString();
                    break;
                case 7:
                    resultString = 'G' + this._y.ToString();
                    break;
                case 8:
                    resultString = 'H' + this._y.ToString();
                    break;
                // handle if x is invalid
                default:
                    throw new Exception('(' + this._x.ToString() + ',' + 
                                        this._y.ToString() + ") is invalid cordinates");
            }

            return resultString;
        }

        // new x must be > 0 and < 9
        public void SetX(int newX)
        {
            if (newX > 0 && newX < 9)
                this._x = newX;
        }

        // new y must be > 0 and < 9
        public void SetY(int newY)
        {
            if (newY > 0 && newY < 9)
                this._y = newY;
        }

        // set cordinates by string (example "A4")
        public void SetCordinatesByString(string newCordinates)
        {
            // check new cordinates
            newCordinates = newCordinates.ToUpper();
            if (newCordinates.Length < 2)
                throw new Exception(newCordinates + "are invalid cordinates");

            // convert x
            switch (newCordinates[0])
            {
                case 'A':
                    this._x = 1;
                    break;
                case 'B':
                    this._x = 2;
                    break;
                case 'C':
                    this._x = 3;
                    break;
                case 'D':
                    this._x = 4;
                    break;
                case 'E':
                    this._x = 5;
                    break;
                case 'F':
                    this._x = 6;
                    break;
                case 'G':
                    this._x = 7;
                    break;
                case 'H':
                    this._x = 8;
                    break;
                // handle if cords are invalid
                default:
                    throw new Exception(newCordinates + "are invalid cordinates");
            }

            // convert y
            int newY = int.Parse(newCordinates[1].ToString());
            if (newY > 8 || newY < 1)
                throw new Exception(newCordinates + "are invalid cordinates");
            else
                _y = newY;
        }

        public int GetX()
        {
            return this._x;
        }

        public int GetY()
        {
            return this._y;
        }

        // check if cords is valid
        public bool IsValid()
        {
            if (this._x < 9 && this._x > 0 && this._y < 9 && this._y > 0)
                return true;
            else
                return false;
        }

        public static ChessCordinates operator +(ChessCordinates firstCordinates, 
                                                 ChessCordinates secondCordinates)
        {
            return new ChessCordinates(firstCordinates.GetX() + secondCordinates.GetX(),
                                       firstCordinates.GetY() + secondCordinates.GetY());
        }

        public static bool operator ==(ChessCordinates firstCordinates,
                                                  ChessCordinates secondCordinates)
        {
            if (firstCordinates.GetX() == secondCordinates.GetX() &&
                firstCordinates.GetY() == secondCordinates.GetY())
                return true;
            else
                return false;
        }

        public static bool operator !=(ChessCordinates firstCordinates,
                                                  ChessCordinates secondCordinates)
        {
            if (firstCordinates.GetX() != secondCordinates.GetX() ||
                firstCordinates.GetY() != secondCordinates.GetY())
                return true;
            else
                return false;
        }

        public ChessCordinates()
        {
            this._x = 1;
            this._y = 1;
        }

        // contsructor don't check x and y!
        public ChessCordinates(int x, int y)
        {
            this._x = x;
            this._y = y;
        }

        public ChessCordinates(string cordinates)
        {
            this.SetCordinatesByString(cordinates);
        }
    }
    


    class Program
    {
        static void Main(string[] args)
        {
            // set start and end cordinates
            System.Console.WriteLine("Input start cordinates:");
            string startCordinatesString = System.Console.ReadLine();
            System.Console.WriteLine("Input end cordinates:");
            string endCordinatesString = System.Console.ReadLine();

            // set KnightExplorer, which will find the way
            KnightExplorer explorer = new KnightExplorer(new ChessCordinates(startCordinatesString),
                                                         new ChessCordinates(endCordinatesString));

            // find the way
            ArrayList way = explorer.FindWay();

            // print it
            way.Reverse();
            System.Console.WriteLine("Result way:");
            foreach(ChessCordinates step in way)
            {
                System.Console.Write(step.ToString() + ", ");
            }

            System.Console.ReadKey();
        }
    }

    // finds the way of Knight from one pole to another
    class KnightExplorer
    {
        // variables:
        private bool[,] _chessMatrix;
        private Queue _ways;
        private ChessCordinates _endCordinates;
        private ChessCordinates[] _shifts; // posible shifts to calculate the pole

        public KnightExplorer(ChessCordinates startCordinates, ChessCordinates endCordinates)
        {
            this._chessMatrix = new bool[8, 8];
            this._ways = new Queue();
            this._endCordinates = endCordinates;
            this._shifts = new ChessCordinates[8] { new ChessCordinates(-2, 1), new ChessCordinates(-1, 2),
                                                    new ChessCordinates(1, 2), new ChessCordinates(2, 1),
                                                    new ChessCordinates(2, -1), new ChessCordinates(1, -2),
                                                    new ChessCordinates(-1, -2), new ChessCordinates(-2, -1)};

            // add start cordinates to ways queue
            ArrayList tempList = new ArrayList();
            tempList.Add(startCordinates);
            this._ways.Enqueue(tempList);
        }

        // set pole as visited
        private void _SetPoint(ChessCordinates poleCordinates)
        {
            if (poleCordinates.GetX() < 9 && poleCordinates.GetX() > 0 
                && poleCordinates.GetY() < 9 && poleCordinates.GetY() > 0)
                this._chessMatrix[poleCordinates.GetX() - 1, poleCordinates.GetY() - 1] = true;
        }

        // helps to check if pole haven't been visited
        // return true if pole have been visited
        // return false if not
        private bool _CheckPoint(ChessCordinates poleCordinates)
        {
            if (poleCordinates.IsValid())
                return this._chessMatrix[poleCordinates.GetX() - 1, poleCordinates.GetY() - 1];
            else
                return false;
        }

        // finds way from start cordinates to end cordinates
        // (breadth-first seacrh)
        // return array with way from end cords to start cords
        public ArrayList FindWay()
        {
            while (true) {
                // eject head way and set point on chess board
                ArrayList headWay = (ArrayList)this._ways.Dequeue();
                _SetPoint((ChessCordinates)headWay[0]);

                // check all reachable poles, add them to ways queue if they are not equal end cords 
                foreach (ChessCordinates shift in _shifts)
                {
                    // calculate new pole
                    ChessCordinates newPole = shift + (ChessCordinates)headWay[0];

                    // if new is possible and pole haven't been visited, add way with it to ways queue
                    if(newPole.IsValid() && (_CheckPoint(newPole) == false))
                    {
                        // set new way
                        ArrayList newWay = new ArrayList() { newPole };
                        newWay.AddRange(headWay);

                        // if exploring reachs end cords, return result
                        if ((ChessCordinates)newWay[0] == _endCordinates)
                            return newWay;
                        // else add new way to ways queues
                        else
                            _ways.Enqueue(newWay);
                    }
                }
                    
            }
        }
    }
}
