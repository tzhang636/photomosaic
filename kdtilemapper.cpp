
#include "kdtilemapper.h"

// KDTileMapper
// 
KDTileMapper::KDTileMapper(const SourceImage& theSource, 
			   const vector<TileImage>& theTiles): 
			   TileMapper(theSource, theTiles)
{}


MosaicCanvas* KDTileMapper::map_tiles() const
{
   // checks if vector of TileImage objects is empty
   if (points.empty())
      return NULL;

   // checks if the SourceImage has any pixels
   if (source.get_rows() < 1 || source.get_columns() < 1)
      return NULL;

   // checks if vector of TileImage objects contains at least one valid tile
   int count_valid = 0;
   map<Point<3>, TileImage>::const_iterator it;

   for (it = tile_avg_map.begin(); it != tile_avg_map.end(); it++)
   {
      if (it->second.is_valid())
	count_valid++; 
   }

   if (count_valid == 0)
      return NULL;

   // ---------------------------------------------------------------------
   // start mapping pixels of source image to tile images

   // creates a new MosaicCanvas 
   MosaicCanvas* canvas_img = new MosaicCanvas(source.get_rows(), 
			        	       source.get_columns());
   
   // creates a new KDTree based on the points vector
   KDTree<3> tile_img_pts(points);

   // iterates through each pixel in the source image
   // creates a temp point that stores the color values of a particular pixel
   // uses findNearestNeighbor to find the nearest point that resembles 
	// the temp point in the points vector
   // uses the map to find the TileImage associated with the nearest point
   // sets that tile in the MosaicCanvas
   for (int i=0; i<source.get_rows(); i++)
   {
      for (int j=0; j<source.get_columns(); j++)
      {
	 Color pixel_color = source.get_region_color(i,j);
         Point<3> pixel_color_pt;
	 pixel_color_pt.set(0, pixel_color.rValue);
	 pixel_color_pt.set(1, pixel_color.gValue);
	 pixel_color_pt.set(2, pixel_color.bValue);

	 // cout << "Got here!" << endl; 

	 Point<3> nearest_pt = tile_img_pts.findNearestNeighbor(pixel_color_pt);

	 TileImage mapped_tile_img = tile_avg_map.find(nearest_pt)->second;

         int ret_val = canvas_img->set_tile(i, j, mapped_tile_img);

	 // checks if setting a tile in the MosaicCanvas fails
	 if (ret_val != 0)
	    return NULL;
      }
   }
   return canvas_img;
}
