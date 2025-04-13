/***************************************************************/
/**                                                           **/
/**   Nome:       Eduardo Gonzalez Silva                      **/
/**   Professor:  Cristina Gomes Fernandes                   **/
/**   Turma:      01                                          **/
/**   NumeroUSP:  15448709                                    **/
/**   Exercicio-Programa 01 - Fractais                        **/
/**                                                           **/
/***************************************************************/

#include <stdio.h>
#include <math.h>

void rotate(FILE *outfile, double angle){
    fprintf(outfile, "%lf rotate\n", angle);
}

void line(FILE *outfile, double length){
    fprintf(outfile, "%lf 0 rlineto\n", length);
}

void move(FILE *outfile, double distance){
    fprintf(outfile, "%lf 0 rmoveto\n", distance);
}

void open(FILE *outfile, int x, int y, double lineSize){
    fprintf(outfile, "<< /PageSize [595 842] >> setpagedevice\n");
    fprintf(outfile, "%lf setlinewidth\n", lineSize);
    fprintf(outfile, "%d %d moveto\n\n", x, y);
}

void close(FILE *outfile){
    fprintf(outfile, "stroke\n");
    fprintf(outfile, "showpage\n");
    fclose(outfile);
}

void kochCurve(FILE *outfile, int order, double size, int angle){
    if(order == 0){
        line(outfile, size);
    }

    else{
        rotate(outfile, angle);
        kochCurve(outfile, order-1, size/3, 0);
        kochCurve(outfile, order-1, size/3, -60);
        kochCurve(outfile, order-1, size/3, 120);
        kochCurve(outfile, order-1, size/3, -60);
    }
}

void drawTriangle(FILE *outfile, double size){
    line(outfile, size);
    rotate(outfile, 120);

    line(outfile, size);
    rotate(outfile, 120);

    line(outfile, size);
    rotate(outfile, 120);
}

void canopyCurve(FILE *outfile, int order, double size, int angle){
    rotate(outfile, angle);
    line(outfile, size);
    
    if(order > 0){
        /*left side*/
        canopyCurve(outfile, order-1, 3*size/4, 20);
        move(outfile, -3*size/4);
        rotate(outfile, -20);

        /*right side*/
        canopyCurve(outfile, order-1, 3*size/4, -20);
        move(outfile, -3*size/4);
        rotate(outfile, 20);
    }
}

void antiFlake(FILE *outfile, int order, double size, int angle){
    kochCurve(outfile, order, size, 0);
    rotate(outfile, -120);

    kochCurve(outfile, order, size, 0);
    rotate(outfile, -120);

    kochCurve(outfile, order, size, 0);
}

void dragonCurve(FILE *outfile, int order, int direction, double length){
    if (order == 0){
            line(outfile, length);
    } 

    else{
        dragonCurve(outfile, order-1, 90, length/sqrt(2));
        rotate(outfile, direction);
        dragonCurve(outfile, order-1, -90, length/sqrt(2));
    }
}

void myCurve(FILE *outfile, int order, double size){
    
    /*
    The 'magical number' 2.17 is not of my liking, however, I gave up on trying
    to find an actual formula.
    Despite being a simple fractal, I quite liked it. I tried to create something 
    of low complexity so we could see the progression on big orders, and I think 
    I did it (this one is linear, which is pretty cool!!).
    I wanted to make a Mundelbrot set, but due to my little knowledge of the 
    subject I would be obligated to base myself in internet codes (something I 
    didn't want to do).
    */

    double angleDeg = 7;
    double angleRad = asin(-1) * angleDeg / 180;
    double shift = (size - (cos(angleRad) * (3*size/4)))/2.17;

    drawTriangle(outfile, size);
    move(outfile, shift);
    rotate(outfile, angleDeg);

    if(order > 0)
        myCurve(outfile, order-1,  5*size/6);
}

int main(){
    int curve, order, i;
    char file[128];
    FILE *outfile;

    scanf("%d %d", &curve, &order);
    scanf("%s", file);

    outfile = fopen(file, "w");

    if(curve == 0){/*anti floke curve*/
        open(outfile, 79, 600, 0.2);

        antiFlake(outfile, order+1, 320, 0);

        close(outfile);
    }

    if(curve == 1){/*canopy curve*/
        open(outfile, 300, 150, 0.2);
        canopyCurve(outfile, order, 100, 90);
        close(outfile);
    }

    if(curve == 2){/*dragon curve*/
        open(outfile, 240, 270, 0.2);

        rotate(outfile, 90);
        for(i = 0; i < order; i++)
            fprintf(outfile, "-45 rotate\n");

        dragonCurve(outfile, order, 90, 350);

        close(outfile);
    }

    if(curve == 3){/*my curve*/
        open(outfile, 100, 250, 1);

        myCurve(outfile, order, 380);
        
        close(outfile);
    }

    return 0;
}