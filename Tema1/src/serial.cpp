#include "common.h"
#include <climits>
using namespace std;

int pret_minim, pret_maxim, iteratii;

// Gets the minimum price for own resource from (celli, cellj) perspective
int getCostMinimResursaProprie(const bool resursa, Cell **& stats, const int n,
		const int celli, const int cellj)
{
	int minim = INT_MAX;
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
		{
			if (stats[i][j].resursa == resursa &&
					manhattan(celli, cellj, i, j) + stats[i][j].pret_resursa < minim)
				minim = stats[i][j].pret_resursa;
		}
	return minim;
}

// Gets the minimum price for complementary resource from (celli, cellj) perspective
int getCostMinimResursaCompl(const bool resursa, Cell **& stats, const int n,
		const int celli, const int cellj)
{
	int minim = INT_MAX;
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
		{
			if (i == celli && j == cellj)
				continue;
			if (stats[i][j].resursa == resursa &&
					manhattan(celli, cellj, i, j) + stats[i][j].pret_resursa < minim)
				minim = stats[i][j].pret_resursa;
		}
	return minim;
}

// Computes and adds minimums to matrix
void addMinCostToMatrix(const int n, Cell **& stats)
{
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
		{
			stats[i][j].cost_compl = getCostMinimResursaCompl(!stats[i][j].resursa,
					stats, n, i, j);
			stats[i][j].cost_minim_resursa = getCostMinimResursaProprie(stats[i][j].resursa,
					stats, n, i, j);
		}
}

// Computes the matrix for next year
void computeNextYear(const int n, Cell **& stats, Cell **& next_year)
{
	addMinCostToMatrix(n, stats);
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
		{
			next_year[i][j].resursa = stats[i][j].resursa;
			if (stats[i][j].buget < stats[i][j].cost_compl)
			{
				next_year[i][j].buget = stats[i][j].cost_compl;
				next_year[i][j].pret_resursa = stats[i][j].pret_resursa +
						stats[i][j].cost_compl - stats[i][j].buget;
			}
			else if (stats[i][j].buget > stats[i][j].cost_compl)
			{
				next_year[i][j].buget = stats[i][j].cost_compl;
				next_year[i][j].pret_resursa = stats[i][j].pret_resursa +
						(stats[i][j].cost_compl - stats[i][j].buget) / 2;
			}
			else
			{
				next_year[i][j].buget = stats[i][j].cost_compl;
				next_year[i][j].pret_resursa = stats[i][j].cost_minim_resursa + 1;
			}

			if (next_year[i][j].pret_resursa < pret_minim)
				next_year[i][j].pret_resursa = pret_minim;
			else if (next_year[i][j].pret_resursa > pret_maxim)
			{
				// respecializare
				next_year[i][j].resursa = !stats[i][j].resursa;
				next_year[i][j].buget = pret_maxim;
				next_year[i][j].pret_resursa = (pret_minim + pret_maxim) / 2;
			}
		}
}

// Writes
void writeOutput(ofstream &file_out, const int n, Cell **& stats)
{

}

// Computes requested data for all years
void computeAllYears(const int n, Cell **& stats, Cell **& next_year)
{
	for (int k = 0; k < iteratii; ++k)
	{
		computeNextYear(n, stats, next_year);
	}
}

int main(int argc, char *argv[])
{
	int n;
	Cell **stats;					// Matricea pentru anul curent
	Cell **next_year;				// Matricea pentru anul urmator

    checkArgs(argc, argv, iteratii);

    ifstream file_in(argv[2], ios::in);
    readInputSize(file_in, n, pret_minim, pret_maxim);

    cerr << "Nr iteratii: " << iteratii << "\n";
    cerr << "n: " << n << " pmin: " << pret_minim << " pmax: " << pret_maxim << endl;

    createMatrix(stats, n);
    createMatrix(next_year, n);

    readInput(file_in, n, stats);
    file_in.close();

    //cerr << "manhattan (0,0) -> (3, 4) = " << manhattan(0, 0, 3, 4);
    //addMinCostToMatrix(n, stats);
    //cerr << "max(4, 10) = " << max(4, 10);
    ofstream file_out(argv[3], ios::out);

    return 0;
}
