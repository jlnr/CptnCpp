#ifndef MAP_HPP
#define MAP_HPP

#include "Gosu.hpp"

const int tileNothing = -1;
const int tileGrass = 0;
const int tileEarth = 1;

using namespace std;

class Map
{
	private:		
		vector<int> tiles;
		int width, height;

		Gosu::Image *sky, *gem;
		vector<Gosu::Image*> tileset;

		vector<CollectibleGem> gemlist;

		Gosu::Graphics *graphics;


	public:
		Map(Gosu::Graphics &graphics, wstring filename)
		{
			this->graphics = &graphics;

			Gosu::imagesFromTiledBitmap(graphics, Gosu::sharedResourcePrefix()+L"data/media/CptnRuby Tileset.png", 60, 60, true, this->tileset);
			this->sky = new Gosu::Image(graphics, Gosu::sharedResourcePrefix()+L"data/media/Sky.png", true);
			this->gem = new Gosu::Image(graphics, Gosu::sharedResourcePrefix()+L"data/media/CptnRuby Gem.png", true);

			vector<string> lines;

			ifstream mapfile(Gosu::wstringToUTF8(filename).c_str(),ios::in);
			if(mapfile.is_open())
			{
				while(!mapfile.eof())
				{
					string line;
					getline(mapfile, line);
					lines.push_back(line);
				}
			}

			this->width = lines[0].length();
			this->height = lines.size()-1;

			this->tiles.resize(this->width * this->height);

			for(int y=0; y < this->height; y++)
			{
				string currentLine = lines[y];

				for(int x=0; x < this->width; x++)
				{
					string c = currentLine.substr(x, 1);

					this->tiles[y * this->width + x] = tileNothing;

					if(c.compare("\"") == 0)
					{
						this->tiles[y * this->width + x] = tileGrass;
					}
					else if(c.compare("#")  == 0)
					{
						this->tiles[y * this->width + x] = tileEarth;
					}
					else if(c.compare("x") == 0)
					{
						this->gemlist.push_back(CollectibleGem(this->gem, x, y));
					}
				}
			}
		}

		~Map()
		{
			delete this->sky; delete this->gem;
		}

		vector<int> getTiles() {
			return this->tiles;
		}

		int getHeight()
		{
			return this->height;
		}

		int getWidth()
		{
			return this->width;
		}

		void draw(float screen_x, float screen_y, bool background = true)
		{
			if(background) this->sky->draw(0, 0, 0);
			
			// draw all of our tiles
			for(int x=0; x < this->width; x++)
			{
				for(int y=0; y < this->height; y++)
				{
					int currentTile = this->tiles[y * this->width + x];
					if(currentTile != tileNothing)
					{
						this->tileset[currentTile]->draw(x * 50 - screen_x, y * 50 - screen_y, 1);
					}
				}
			}

			for(vector<CollectibleGem>::iterator it = this->gemlist.begin(); it != gemlist.end(); ++it)
			{
				(*it).draw(screen_x, screen_y);
			}


		}

		bool isSolid(int x, int y)
		{
			return (y < 0 || this->tiles[ ( (y+50)/50) * this->width + (x/50)] >= 0);
		}
};

#endif