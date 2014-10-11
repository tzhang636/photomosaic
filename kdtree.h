/* kdtree.h
 * KDTree implementation using Points in k-dimensional space.
 *
 * Written by Cinda Heeren for CS225 MP6
 * Spring 2009
 * 
 * Revision history:
 * 3/31/2009        Created
*/

#ifndef __KDTREE_H__
#define __KDTREE_H__

#include <vector>
#include "point.h"

using namespace std;

template<int Dim>
class KDTree;

template<int Dim>
ostream & operator<<(ostream & out, KDTree<Dim> const & tree);

template<int Dim>
class KDTree
{
   private:
      vector< Point<Dim> > points;
      vector<int> point_index;

      void cctrHelper(int lo, int hi, int k, int curr_dim);
      void select(int lo, int hi, int k, int curr_dim);
      int partition(int lo, int hi, int pivotIndex, int curr_dim);

      int fNNHelper(int lo, int hi, int curr_dim, 
		    const Point<Dim> & source) const;
      int findDist(const Point<Dim> & temp, const Point<Dim> & source) const;

   public:
      KDTree(const vector< Point<Dim> > & source);
      Point<Dim> findNearestNeighbor(const Point<Dim> & source) const;      

      friend ostream & operator<< <Dim>(ostream & out, 
					KDTree<Dim> const & tree);
};

#include "kdtree.cpp"

#endif

