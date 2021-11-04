// Fractals.cpp : Defines the entry point for the console application.
//
// hillo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;
using namespace sf;

const int imagew = 1080; //window constraints
const int imageh = 800;
const int maxarray = 3;
const int maxcount = 100;

struct colour                    //A struct so that we can create a variation of colours on the generated image
{
    int r;
    int g;
    int b;
};
struct complex
{
    double real;                //A struct to have seperate the real and imaginary parts in a complex number.
    double imaginary;
};


colour mandelbrot(int x, int y)
{
    int count = 0;
    double minreal = -1.5, maxreal = 1;                //bounds to create the scale of the graph since its being done like a graph
    double minimag = -1, maximag = 1;

    double creal = minreal + (x*(maxreal - minreal) / imagew);            //putting points into scale so that it can be plotted on x - y plane
    double cimag = minimag + (y*(maximag - minimag) / imageh);            //the function is f(z)=z^2+c. c is complex nukmber so we have to seperate it
    double x1 = 0, y1 = 0;                                                                    //c= x+yi. i  is -1.


    while (x1*x1 + y1 * y1 <= 16 && count < maxcount)
    {
        double newx = x1 * x1 - y1 * y1 + creal;                //gives the new z value and iterates itself 
        y1 = 2 * x1*y1 + cimag;
        x1 = newx;
        count++;
    }
    if (count < maxcount)
    {
        return { (255 * count) / 20,(128 * count) / 20,0 };                                    //divided so that we can a variation of the red or whatever colour the variations depend on how fast it broke the count
    }
    else
    {
        return { 0,0,0 };
    }



}
colour julia(int x, int y, complex num, int colouring = 0)
{
    int count = 0;
    int maxcount = 100;
    double minreal = -1.5, maxreal = 1.5;
    double minimag = -2, maximag = 2;

    //scales the pixels into the complex plane so that we can work with points
    double zx = minreal + (x*(maxreal - minreal)) / imagew;
    double zy = minimag + (y*(maximag - minimag)) / imageh;

    while (zx*zx + zy * zy < 4 && count < maxcount)
    {
        double newx = zx * zx - zy * zy + num.real;                //gives the new z value and iterates itself 
        zy = 2 * zx*zy + num.imaginary;                            //the differnce between the last function and this one is that the complex number had to be passed into it which is what causes the variation
        zx = newx;
        count++;
    }

    if (count == maxcount)
    {
        return { 0,0,0 };
    }
    else {
        switch (colouring)
        {
            //returning the rgb of the pixel that will be set on the image
            case 0:return{ (255 * count) / 20, 0, 0 };
            case 1:return{ (255 * count) / 20,(128 * count) / 20,0 };
            case 2:return{ (255 * count) / 20,(255 * count) / 20,0 };
            case 3:return{ (128 * count) / 20,(255 * count) / 20,0 };
            case 4:return{ 0,(255 * count) / 20,0 };
            case 5:return{ 0,(255 * count) / 20,(255 * count) / 20 };
            case 6:return{ 0,0,(255 * count) / 20 };
            case 7:return{ (127 * count) / 20,0,(255 * count) / 20 };
            case 8:return{ (255 * count) / 20,0,(255 * count) / 20 };
            case 9:return{ (128 * count) / 20,(128 * count) / 20,(128 * count) / 20 };
        }
    }
}
void openwin(sf::Image pic)
{
    cout << "\nTo exit the window you can press either ESC or the close button." << endl;
    RenderWindow window(VideoMode(imagew, imageh), "Fractals");
    Texture letsgo;
    Sprite lilgo;
    //Function to open a window 
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            //only closes when the close button is pressed otherwise it would blink away
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed)
            {
                //can be closed by pressing ESC key
                if (event.key.code == Keyboard::Escape)
                {
                    window.close();
                }
            }

        }
        letsgo.loadFromImage(pic);        //the image is put into the texture
        lilgo.setTexture(letsgo);
        window.draw(lilgo);
        window.display();                //Used to display the image
    }
}

void Userfractal(vector<complex> anything, int color = 0)
{
    Image image;
    image.create(imagew, imageh, sf::Color::White);
    ofstream numbers("numbers.txt");
    cout << "Here you can put in your own random numbers to create your own fractal." << endl;
    for (int n = 0; n < anything.size(); n++)
    {
        cout << "Please enter your first number: ";
        cin >> anything[n].real;
        while (anything[n].real < -1 || anything[n].real>1)
        {
            cout << "It has to be a decimal between 1 and -1\nTry again: ";
            cin >> anything[n].real;
        }

        cout << "One more time: ";
        cin >> anything[n].imaginary;
        while (anything[n].imaginary < -1 || anything[n].imaginary>1)
        {
            cout << "It's not in the range.\nTry again: ";
            cin >> anything[n].imaginary;
        }

        numbers << anything[n].real << " " << anything[n].imaginary << endl;;
        cout << "Enjoy the fractal you've made. :)" << endl;

        for (int y = 0; y < imageh - 1; y++)
        {
            for (int x = 0; x < imagew - 1; x++)
            {
                colour value = julia(x, y, anything[n], color);
                image.setPixel(x, y, sf::Color(value.r, value.g, value.b));
            }
        }
        openwin(image);
    }
    numbers.close();

}
void genfractal(complex value, int colourchoice = 0)
{
    Image image;
    int choice = colourchoice;
    image.create(imagew, imageh, Color::Black);
    for (int x = 0; x < imagew - 1; x++)
    {
        for (int y = 0; y < imageh - 1; y++)
        {
            colour values = julia(x, y, value, choice);
            image.setPixel(x, y, sf::Color(values.r, values.g, values.b));
        }

    }
    openwin(image);
    image.saveToFile("Fractal.png");
}
void whatcolour(string asda, complex num)
{
    bool flag = false;
    string choice = asda;
    int colourpick;
    while (!flag)
    {
        if (choice == 'y')
        {
            cout << "Ok, I'm glad you're happy." << endl;
            flag = true;
        }
        else if (choice == 'n')
        {
            cout << "Alright, you can pick again." << endl;
            while (choice == 'n')
            {
                cout << "What colour would you like the Fractal to be?" << endl;
                cout << "From 0-9, you can pick between:\nRed = 0\nOrange = 1\nYellow = 2\nLime = 3\nGreen = 4\nTurquiose = 5\nBlue = 6\nPurple = 7\nPink = 8\nGray = 9" << endl;
                cout << "Please enter a number: ";
                cin >> colourpick;
                while (colourpick > 9 || colourpick < 0)
                {
                    cout << "That's not a valid option." << endl;
                    cin >> colourpick;
                }
                genfractal(num, colourpick);
                cout << "Are you happy with what you've made?\nY/N?" << endl;
                cin >> choice;
            }
        }
        else {
            cout << "Not a valid response.\nPlease type in 'y' or 'n' if you are done or would like to pick again, respctively." << endl;
            cin >> choice;
        }
    }
}


void fract()
{
    sf::Image image;
    image.create(imagew, imageh, sf::Color::White);

    //flick through pixels in the window to be put into the function
    for (int y = 0; y < imageh - 1; y++)
    {
        for (int x = 0; x < imagew - 1; x++)
        {
            colour value = mandelbrot(x, y);
            image.setPixel(x, y, sf::Color(value.r, value.g, value.b));
        }
    }

    cout << "Here we have the mandelbrot set." << endl;
    openwin(image);

    int arraysize;
    cout << "So this program will allow you to create fractals of your own." << endl;
    cout << "\nHow many images would you like to make: ";
    //Retrieving the size of the array from user
    cin >> arraysize;
    string listing;
    string real1;
    string imag1;
    //Reading from a file that has a few recommended values to input
    ifstream ex("Examples.txt");
    cout << "\nThis is a list of recommended images." << endl;
    cout << setw(8) << "First" << setw(8) << "Second" << endl;
    for (int i = 0; !ex.eof(); i++)
    {
        getline(ex, listing);
        istringstream list(listing);
        //Used so that the string retrieved can be separated by spaces
        vector<string>advice(istream_iterator<string>{list}, istream_iterator<string>());
        real1 = advice[0];
        imag1 = advice[1];
        cout << setw(8) << real1 << setw(8) << imag1 << endl;
    }

    image.saveToFile("image2.bmp");                                    //like function says it saves to file name

    cout << "For this program to work you are bounded by 1 and -1.\nAnd try to keep it as a decimal for the best results." << endl;

    complex array[maxarray];
    vector<complex> compnums(arraysize);
    int count = 0;
    int incre = 0;

    Userfractal(compnums);
    ifstream getnum("numbers.txt");
    string line;
    double real;
    double imag;
    for (int i = 0; i < arraysize; i++)
    {
        //Gets the string from file - file contains the values the user input
        getline(getnum, line);
        istringstream iss(line);
        //Only stores the first line from the file, which would be the real and imaginary part of the complex number
        vector<string>results(istream_iterator<string>{iss}, istream_iterator<string>());
        real = stod(results[0]);
        compnums[i].real = real;
        cout << real << endl;
        imag = stod(results[1]);
        compnums[i].imaginary = imag;
        cout << imag << endl;
    }
    getnum.close();
    for (int n = 0; n < compnums.size(); n++)
    {
        cout << compnums[n].real << " " << compnums[n].imaginary << endl;
        cout << compnums.size() << endl;
    }

    cout << "So we're gonna flick through all the fractals you have created and you can save the one you like best." << endl;
    for (int n = 0; n < arraysize; n++)
    {
        for (int y = 0; y < imageh - 1; y++)
        {
            for (int x = 0; x < imagew - 1; x++)
            {
                colour something = julia(x, y, compnums[n]);
                image.setPixel(x, y, Color(something.r, something.g, something.b));
            }
        }
        cout << "This is fractal number " << n + 1 << endl;
        openwin(image);
    }
    cout << "You can choose which of the fractals you would like to save.\nJust choose between 1 and " << compnums.size() << endl;
    int choice = 0;
    cin >> choice;
    while (choice > compnums.size() || choice < 0)
    {
        cout << "It's not an option.\nPlease try again." << endl;
        cin >> choice;
    }
    complex number = compnums[choice - 1];
    for (int y = 0; y < imageh; y++)
    {
        for (int x = 0; x < imagew; x++)
        {
            colour value = julia(x, y, number);
            image.setPixel(x, y, sf::Color(value.r, value.g, value.b));
        }
    }
    image.saveToFile("image1.jpg");
    cout << "You can mess arround with the colours of the fractal that you have saved if you would like." << endl;
    bool flag = false;
    string yesorno;
    int colourpick;
    while (!flag)
    {
        cout << "What colour would you like the Fractal to be?" << endl;
        cout << "From 0-9, you can pick between:\nRed = 0\nOrange = 1\nYellow = 2\nLime = 3\nGreen = 4\nTurquiose = 5\nBlue = 6\nPurple = 7\nPink = 8\nGray = 9" << endl;
        cout << "Please enter a number: ";
        cin >> colourpick;
        genfractal(number, colourpick);
        cout << "If you are content with your colour choice, type in either 'y' if you are done and 'n' if you want to pick another colour." << endl;
        cin >> yesorno;
        whatcolour(yesorno, compnums[choice - 1]);
        flag = true;
    }
    cout << "The fractal you have made has been saved. :)" << endl;
}

int main()
{
    fract();
    return 0;
}

