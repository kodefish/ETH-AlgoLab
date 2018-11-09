#include <iostream>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

// Define exact type
typedef CGAL::Gmpq ET;

// Program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

double floor_to_double(CGAL::Quotient<ET> x) {
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a--;
  while (a + 1 <= x) a++;
  return a;
}

void testcase(int n, int m) {
  Program lp(CGAL::LARGER, true, 0, false, 0);

  for (int i = 0; i < n; i++) {
    int min_nutrient, max_nutrient; std::cin >> min_nutrient >> max_nutrient;
    lp.set_b(i, min_nutrient); lp.set_r(i, CGAL::LARGER); // >= min_nutrient
    lp.set_b(i + n, max_nutrient); lp.set_r(i + n, CGAL::SMALLER); // <= max_nutrient
  }

  for (int i = 0; i < m; i++) {
    int price_food; std::cin >> price_food;
    lp.set_c(i, price_food);
    for (int j = 0; j < n; j++) {
      int nutrient_amount; std::cin >> nutrient_amount;
      lp.set_a(i, j, nutrient_amount);
      lp.set_a(i, j + n, nutrient_amount);
    }
  }

  Solution s = CGAL::solve_linear_program(lp, ET());
  if (s.is_infeasible()) {
    std::cout << "No such diet." << std::endl;
  } else {
    std::cout << std::setprecision(12) << floor_to_double(s.objective_value()) << std::endl;
  }

}

int main() {
  int num_nutrients, num_foods; std::cin >> num_nutrients >> num_foods;
  while (num_nutrients != 0 && num_foods != 0) {
    testcase(num_nutrients, num_foods);
    std::cin >> num_nutrients >> num_foods;
  }
}
