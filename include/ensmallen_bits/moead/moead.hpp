/**
 * @file moead.hpp
 * @author Utkarsh Rai
 *
 * MOEA/D, Multi Objective Evolutionary Algorithm based on Decompositon is a
 * multi objective optimization algorithm. It employs evolutionary algorithms,
 * to find better solutions by iterating on the previous solutions and
 * decomposition approaches, to convert the multi objective problem to a single
 * objective one, to find the best Pareto Front for the given problem.
 *
 * ensmallen is free software; you may redistribute it and/or modify it under
 * the terms of the 3-clause BSD license.  You should have received a copy of
 * the 3-clause BSD license along with ensmallen.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */

#ifndef ENSMALLEN_MOEAD_MOEAD_HPP
#define ENSMALLEN_MOEAD_MOEAD_HPP

namespace ens {

/**
 * This class implements the MOEA/D algorithm. Step numbers used in different
 * parts of the implementation correspond to the step number used in the
 * original algorithm by the author.
 *
 * For more information, see the following:
 * @code
 * @articl{article,
 * author = {Zhang, Qingfu and Li, Hui},
 * year = {2008},
 * month = {01},
 * pages = {712 - 731},
 * title = {MOEA/D: A Multiobjective Evolutionary Algorithm Based on
 *    Decomposition},
 * volume = {11},
 * journal = {Evolutionary Computation, IEEE Transactions on},
 * doi = {10.1109/TEVC.2007.892759}}
 * @endcode
 *
 * MOEA/D can optimize arbitrary multi-objective functions. For more details,
 * see the documentation on function types included with this distribution or
 * on the ensmallen website.
 */

class MOEAD {
 public:
  /**
   * Constructor for the MOEA/D optimizer.
   *
   * The default values provided here are not necessarily suitable for a
   * given function. Therefore, it is highly recommended to adjust the
   * parameters according to the problem.
   *
   * @param populationSize, the number of elements in the population.
   * @param crossoverProb The probability that a crossover will occur.
   * @param mutationProb The probability that a mutation will occur.
   * @param neighbourhoodSize, the number of nearest neighbours of weights
   *    to find.
   * @param lowerBound, the lower bound on each variable of a member
   *    of the variable space.
   * @param upperBound, the upper bound on each variable of a member
   *    of the variable space.
   */
  MOEAD(const size_t populationSize = 100,
        const double crossoverProb = 0.6,
        const double mutationProb = 0.3,
        const double mutationStrength = 1e-3,
        const size_t neighbourhoodSize = 50,
        const arma::vec& lowerBound = arma::ones(1, 1),
        const arma::vec& upperBound = arma::ones(1, 1));
  /**
   * Optimize a set of objectives. The initial population is generated
   * using the initial point. The output is the best generated front.
   *
   * @tparam MatType, the type of matrix used to store coordinates.
   * @tparam ArbitraryFunctionType, the type of objective function.
   * @tparam CallbackTypes, types of callback function.
   * @param objectives, std::tuple of the objective functions.
   * @param iterate, the initial reference point for generating population.
   * @param callbacks, callback functions.
   */
  template<typename MatType,
           typename... ArbitraryFunctionType,
           typename... CallbackTypes>
  typename MatType::elem_type Optimize(std::tuple<ArbitraryFunctionType...>& objectives,
                              MatType& iterate,
                              CallbackTypes&&... callbacks);

  //! Get the population size.
  size_t PopulationSize() const { return populationSize; }
  //! Modify the population size.
  size_t& PopulationSize() { return populationSize; }

  //! Get the crossover rate.
  double CrossoverRate() const { return crossoverProb; }
  //! Modify the crossover rate.
  double& CrossoverRate() { return crossoverProb; }

  //! Get the mutation probability.
  double MutationProbability() const { return mutationProb; }
  //! Modify the mutation probability.
  double& MutationProbability() { return mutationProb; }

  //! Get the size of the weight neighbourhood.
  size_t NeighbourhoodSize() const { return neighbourhoodSize; }
  //! Modify the size of the weight neighbourhood.
  size_t& NeighbourhoodSize() { return neighbourhoodSize; }

  //! Retrieve value of lowerBound.
  const arma::vec& LowerBound() const { return lowerBound; }
  //! Modify value of lowerBound.
  arma::vec& LowerBound() { return lowerBound; }

  //! Retrieve value of upperBound.
  const arma::vec& UpperBound() const { return upperBound; }
  //! Modify value of upperBound.
  arma::vec& UpperBound() { return upperBound; }

  //! Retrieve the best front (the Pareto frontier).  This returns an empty
  //! vector until `Optimize()` has been called.
  const std::vector<arma::mat>& Front() const { return bestFront; }

 private:
  /**
   * Mutate child formed by the crossover of two random members of the 
   * population.
   *
   * @tparam MatType, the type of matrix used to store coordinates.
   * @param child, the matrix to mutate.
   * @param lowerBound, the lower bound on each variable in the matrix.
   * @param upperBound, the upper bound on each variable in the matrix.
   */
  template<typename MatType>
  void Mutate(MatType& child,
              const arma::vec& lowerBound,
              const arma::vec& upperBound);

  /**
   * Decompose the multi objective problem to a single objetive problem
   * using the Tchebycheff approach.
   *
   * @param weights, a set of real number which act as weights.
   * @param idealPoint, ideal point z in Tchebycheff decomposition.
   * @param evaluatedCandidate, value of the candidate per objective.
   * @return the single value obtained from decomposed function.
   */
  double DecomposedSingleObjective(const arma::vec& weights,
                                   const arma::vec& idealPoint,
                                   const arma::vec& evaluatedCandidate);

  /**
   * Check domination between two vectors.
   *
   * @param first, the first of the two vectors to compare.
   * @param second, the second of the two vectors to compare.
   * @return true if first dominates second else false.
   */
  bool Dominates(const arma::vec& first,
                 const arma::vec& second);

  /**
   * Evaluate objectives for the elite population.
   *
   * @tparam ArbitraryFunctionType std::tuple of multiple function types.
   * @tparam MatType Type of matrix to optimize.
   * @param population The elite population.
   * @param objectives The set of objectives.
   * @param calculatedObjectives Vector to store calculated objectives.
   */    
  template<std::size_t I = 0,
           typename MatType,
           typename ...ArbitraryFunctionType>
  typename std::enable_if<I == sizeof...(ArbitraryFunctionType), void>::type
  EvaluateObjectives(std::vector<MatType>&,
      std::tuple<ArbitraryFunctionType...>&,
      arma::mat&);

  template<std::size_t I = 0,
           typename MatType,
           typename ...ArbitraryFunctionType>
  typename std::enable_if<I < sizeof...(ArbitraryFunctionType), void>::type
  EvaluateObjectives(std::vector<MatType>& population,
      std::tuple<ArbitraryFunctionType...>& objectives,
      arma::mat& calculatedObjectives);

  template<std::size_t I = 0,
           typename MatType,
           typename ...ArbitraryFunctionType>
  typename std::enable_if<I == sizeof...(ArbitraryFunctionType), void>::type
  EvaluateObjectives(std::vector<MatType>&,
      std::tuple<ArbitraryFunctionType...>&,
      std::vector<arma::vec>&);

  template<std::size_t I = 0,
           typename MatType,
           typename ...ArbitraryFunctionType>
  typename std::enable_if<I < sizeof...(ArbitraryFunctionType), void>::type
  EvaluateObjectives(std::vector<MatType>& population,
      std::tuple<ArbitraryFunctionType...>& objectives,
      std::vector<arma::vec>& calculatedObjectives);

  //! Size of the population.
  size_t populationSize;

  //! Probability of crossover between two members.
  double crossoverProb;

  //! Probability of mutation of a child.
  double mutationProb;

  //! Strength of mutation.
  double mutationStrength;

  //! Number of nearest neighbours of weights to consider.
  size_t neighbourhoodSize;

  //! Lower bound on each variable in the variable space.
  arma::vec lowerBound;

  //! Upper bound on each variable in the variable space.
  arma::vec upperBound;

  //! The number of objectives in multi objective optimisation problem.
  size_t numObjectives;

  //! The Pareto Optimal Front.
  std::vector<arma::mat> bestFront;
};

} // namespace ens

// Include implementation.
#include "moead_impl.hpp"

#endif