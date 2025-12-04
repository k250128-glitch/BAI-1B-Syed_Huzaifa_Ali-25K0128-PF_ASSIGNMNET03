#include <stdio.h>

void calculateFuel(int fuel, int consumption, int recharge, int solarBonus,
                   int planet, int totalPlanets)
{
    // Base cases
    if (fuel <= 0) {
        printf("Fuel depleted! Mission aborted at Planet %d.\n", planet);
      printf("HUZAIFA THE PILOT HAS LOST FUEL\n");
        return;
    }
    if (planet > totalPlanets) {
        printf("Mission successful! Spacecraft completed the journey.\n");
         printf("HUZIAFA THE PILOT HAS COMPLETED HIS JOURNEY");
        return;
    }

    // Apply consumption
    fuel -= consumption;

    // Gravitational recharge
    fuel += recharge;

    // Solar recharge after every 4th planet
    if (planet % 4 == 0) {
        fuel += solarBonus;
    }

    int displayFuel = (fuel > 0 ? fuel : 0);

    printf("Planet %d: Fuel Remaining = %d\n", planet, displayFuel);


    calculateFuel(fuel, consumption, recharge, solarBonus,
                  planet + 1, totalPlanets);
}

int main()
{
    int initialFuel, consumption, recharge, solarBonus, totalPlanets;

    printf("Enter initial fuel: ");
    scanf("%d", &initialFuel);

    printf("Enter fuel consumption per planet: ");
    scanf("%d", &consumption);

    printf("Enter gravitational recharge amount: ");
    scanf("%d", &recharge);

    printf("Enter solar recharge bonus (every 4th planet): ");
    scanf("%d", &solarBonus);

    printf("Enter total number of planets to visit: ");
    scanf("%d", &totalPlanets);

    printf("\n--- Spacecraft Fuel Simulation ---\n");
    calculateFuel(initialFuel, consumption, recharge, solarBonus, 1, totalPlanets);
// PRogram by Syed  Huzaifa Ali
printf("THE PROGRAM WAS MADE BY SYED HUZAIFA ALI\n 25K-0128");
    return 0;
}

