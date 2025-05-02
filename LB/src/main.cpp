#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

const int nx = 100;       // Lattice size in x
const int ny = 50;        // Lattice size in y
const int Q = 9;          // Number of velocity directions (D2Q9)
const double tau = 0.6;   // Relaxation time
const double w[Q] = {4.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 
                     1.0 / 36.0, 1.0 / 36.0, 1.0 / 36.0, 1.0 / 36.0}; // Weights
const int cx[Q] = {0, 1, 0, -1, 0, 1, -1, -1, 1}; // x velocities
const int cy[Q] = {0, 0, 1, 0, -1, 1, 1, -1, -1}; // y velocities

// Function to compute equilibrium distribution
double feq(int k, double rho, double ux, double uy) {
    double cu = cx[k] * ux + cy[k] * uy;
    double u2 = ux * ux + uy * uy;
    return w[k] * rho * (1.0 + 3.0 * cu + 4.5 * cu * cu - 1.5 * u2);
}

int main() {
    // Initialize distributions and macroscopic variables
    std::vector<std::vector<std::vector<double>>> f(Q, std::vector<std::vector<double>>(nx, std::vector<double>(ny, 1.0 / 9.0)));
    std::vector<std::vector<double>> rho(nx, std::vector<double>(ny, 1.0));
    std::vector<std::vector<double>> ux(nx, std::vector<double>(ny, 0.0));
    std::vector<std::vector<double>> uy(nx, std::vector<double>(ny, 0.0));

    int nsteps = 500; // Number of timesteps
    for (int step = 0; step < nsteps; ++step) {
        // Collision step
        for (int x = 0; x < nx; ++x) {
            for (int y = 0; y < ny; ++y) {
                // Compute macroscopic quantities
                rho[x][y] = 0.0;
                ux[x][y] = 0.0;
                uy[x][y] = 0.0;
                for (int k = 0; k < Q; ++k) {
                    rho[x][y] += f[k][x][y];
                    ux[x][y] += f[k][x][y] * cx[k];
                    uy[x][y] += f[k][x][y] * cy[k];
                }
                ux[x][y] /= rho[x][y];
                uy[x][y] /= rho[x][y];
                for (int k = 0; k < Q; ++k) {
                    f[k][x][y] = f[k][x][y] - (f[k][x][y] - feq(k, rho[x][y], ux[x][y], uy[x][y])) / tau;
                }
            }
        }

        // Streaming step
        std::vector<std::vector<std::vector<double>>> f_temp = f;
        for (int k = 0; k < Q; ++k) {
            for (int x = 0; x < nx; ++x) {
                for (int y = 0; y < ny; ++y) {
                    int x_new = (x + cx[k] + nx) % nx;
                    int y_new = (y + cy[k] + ny) % ny;
                    f[k][x_new][y_new] = f_temp[k][x][y];
                }
            }
        }

        // Boundary conditions: Simple bounce-back at the walls
        for (int y = 0; y < ny; ++y) {
            for (int k = 0; k < Q; ++k) {
                f[k][0][y] = f[3][1][y];         // Left wall
                f[k][nx - 1][y] = f[1][nx - 2][y]; // Right wall
            }
        }

        for (int x = 0; x < nx; ++x) {
            for (int k = 0; k < Q; ++k) {
                f[k][x][0] = f[4][x][1];         // Bottom wall
                f[k][x][ny - 1] = f[2][x][ny - 2]; // Top wall
            }
        }

        // Visualization or output (optional)
        if (step % 50 == 0) {
            std::cout << "Step " << step << ": Average rho = " << rho[nx / 2][ny / 2] << std::endl;
        }
    }

    // Save final state for visualization
    std::ofstream file("velocity.dat");
    for (int x = 0; x < nx; ++x) {
        for (int y = 0; y < ny; ++y) {
            file << x << " " << y << " " << ux[x][y] << " " << uy[x][y] << "\n";
        }
    }
    file.close();

    std::cout << "Simulation completed!" << std::endl;

    return 0;
}
