/* kdtree.cpp
 * Implementation of KDTree class
 * 
 * Written by Cinda Heeren for CS225 MP6
 * Spring 2009
 * Revision history:
 * 3/31/2009        Created
*/

// define KDTree member functions here

#include <cmath>

template <int Dim>
KDTree<Dim>::KDTree(const vector< Point<Dim> > & source)
{
   points.resize((int)source.size());
   point_index.resize((int)source.size());

   for (int i=0; i<(int)points.size(); i++)
   {
      for (int j=0; j<Dim; j++)
         points[i].set(j, source[i][j]);
   }

   for (int i=0; i<(int)point_index.size(); i++)
      point_index[i] = i;

   cctrHelper(0, (int)point_index.size()-1, (0+(int)point_index.size()-1)/2, 0);
   // median = (lo + hi) / 2
}


template <int Dim>
void KDTree<Dim>::cctrHelper(int lo, int hi, int k, int curr_dim)
{
   if (lo >= hi)
      return;

   select(lo, hi, k, curr_dim);

   cctrHelper(lo, k-1, (lo+(k-1))/2, (curr_dim+1)%Dim);
   cctrHelper(k+1, hi, ((k+1)+hi)/2, (curr_dim+1)%Dim);
}


template <int Dim>
void KDTree<Dim>::select(int lo, int hi, int k, int curr_dim)
{
   int pivotIndex = (lo+hi)/2; // choose arbitrary index
   int pivotNewIndex = partition(lo, hi, pivotIndex, curr_dim);

   if (k == pivotNewIndex)
      return;
   else if (k < pivotNewIndex)
      select(lo, pivotNewIndex-1, k, curr_dim);
   else
      select(pivotNewIndex+1, hi, k, curr_dim); 
}


template <int Dim>
int KDTree<Dim>::partition(int lo, int hi, int pivotIndex, int curr_dim)
{
   double pivotValue = points[point_index[pivotIndex]][curr_dim];

   int temp1 = point_index[pivotIndex];
   point_index[pivotIndex] = point_index[hi];
   point_index[hi] = temp1;

   int storeIndex = lo;

   for (int i=lo; i<hi; i++)
   {
      if (points[point_index[i]][curr_dim] < pivotValue)
      {
	 int temp2 = point_index[storeIndex];
	 point_index[storeIndex] = point_index[i];
         point_index[i] = temp2;
         
         storeIndex = storeIndex + 1;
      }
   }

   int temp3 = point_index[hi];
   point_index[hi] = point_index[storeIndex];
   point_index[storeIndex] = temp3;

   return storeIndex;       
}


template<int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim> & source) const
{
   Point <Dim> temp;
   for (int i=0; i<Dim; i++)
   {
      temp.set(i, 
      points[fNNHelper(0, (int)point_index.size()-1, 0, source)][i]);
   }
   return temp;
}


template<int Dim>
int KDTree<Dim>::fNNHelper(int lo, int hi, int curr_dim, 
		    	   const Point<Dim> & source) const
{
   if (lo >= hi)
      return point_index[lo];

   int median = (lo+hi)/2;

   if (points[point_index[median]][curr_dim] < source[curr_dim])
   {
      int index = fNNHelper(median+1, hi, (curr_dim+1)%Dim, source);

      int dist = findDist(points[index], source);
      int dist1 = findDist(points[point_index[median]], source);

      if (dist1 < dist){
         index = point_index[median];
      }

      if ((int)pow(points[index][curr_dim]-source[curr_dim], 2) < 
           findDist(points[index], source))
      {
	 int index1 = fNNHelper(lo, median-1, (curr_dim+1)%Dim, source);

         dist = findDist(points[index], source);
         dist1 = findDist(points[index1], source);
         if (dist1 < dist)
            return index1;
      }
      return index;
   }

   else{
      int index = fNNHelper(lo, median-1, (curr_dim+1)%Dim, source);

      int dist = findDist(points[index], source);
      int dist1 = findDist(points[point_index[median]], source);

      if (dist1 < dist)
         index = point_index[median];

      if ((int)pow(points[index][curr_dim]-source[curr_dim], 2) < 
           findDist(points[index], source))
      {
	 int index1 = fNNHelper(median+1, hi, (curr_dim+1)%Dim, source);

         dist = findDist(points[index], source);
         dist1 = findDist(points[index1], source);
         if (dist1 < dist)
            return index1;
      }
      return index;
   }
}


template<int Dim>
int KDTree<Dim>::findDist(const Point<Dim> & temp, const Point<Dim> & source) 
const
{
   int dist = 0;
   for (int i=0; i<Dim; i++)
      dist = dist + (int)pow(temp[i] - source[i], 2);
   return dist;   
}


template<int Dim>
std::ostream & operator<<(std::ostream & out, KDTree<Dim> const & tree)
{
   for (unsigned int i = 0; i < tree.points.size(); ++i)
   {
      out << "Point #"<< i << " coords = (";
      for (unsigned int j = 0; j < Dim; ++j)
      {
         if (j > 0)
            out << ", ";
         out << tree.points[i][j];
      }
      out << ") index = " << tree.point_index[i] << std::endl;
   }
   return out;
}
