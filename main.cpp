#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

#define L 300
#define T 50000

bool instable(int* M){
    for (int i=0; i<L*L; ++i){
        if (M[i]>3) return true;
    }
    return false;
}

std::vector<int> overloaded(int* M){
    std::vector<int> Index;
    for (int i=0; i<L*L; ++i){
        if (M[i]>3) Index.push_back(i);
    }
    return Index;
}

int main() {

    int M[L*L];

    for (int i=0; i<L*L; ++i){
        M[i]=0;
    }

    FILE *fp = fopen("M", "w");

    for (int t = 0; t < T; ++t) {

        std::cout << t << std::endl;
        M[L*L/2 + L/2] = M[L*L/2 +L/2] + 1;

        while ( instable(M) ) {

            std::vector Indexes=overloaded(M);
            int randomIndex = Indexes[rand()%size(Indexes)];

            M[randomIndex] = M[randomIndex] - 4;

            if (randomIndex != L-1 ) M[randomIndex+1] = M[randomIndex+1] + 1;
            if (randomIndex != 0 )   M[randomIndex-1] = M[randomIndex-1] + 1;
            if (randomIndex < L*L - L ) M[randomIndex+L] = M[randomIndex+L] + 1;
            if (randomIndex > L-1 ) M[randomIndex-L] = M[randomIndex-L] + 1;

        }
    }

    /*for (int i = 1; i < L*L; ++i) {
        if (i%L == 0) {
            fprintf(fp, "\n %d", M[i]);
        } else {
            fprintf(fp, "\t %d", M[i]);
        }
    }*/

    std::ofstream output("M2.ascii");
    for (int k=1; k<(L*L); k++)
    {
            output << M[k] << " "; // behaves like cout - cout is also a stream
        if (k%L==0)output << "\t" << std::endl;
    }

    output << 0 << " ";

    return 0;
}
