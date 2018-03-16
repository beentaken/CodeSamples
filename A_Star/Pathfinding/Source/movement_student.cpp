/* Copyright Steve Rabin, 2012. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright Steve Rabin, 2012"
 */

#include <Stdafx.h>

enum OnList {Open, Close, None};

typedef struct
{
	int row_parent, col_parent; //Parent
	float cost;				//Total cost
	float given;			//Given cost
	OnList my_list;			//Which list the node is on
	std::list<std::pair<int, int> > neighbours;
}Node;

void ZeroNodes()
{
	for (int i = 0; i < g_terrain.GetWidth(); ++i)
	{
		for (int j = 0; j < g_terrain.GetWidth(); ++j)
		{
			g_map[i][j].given = 0;
			g_map[i][j].cost = 0;
			g_map[i][j].my_list = None;
		}
	}
	
}

void Precompute()
{
	g_map = new Node*[g_terrain.GetWidth()];
	for (int i = 0; i < g_terrain.GetWidth(); ++i)
		g_map[i] = new Node[g_terrain.GetWidth()];

	//Precomputing neighbours
	for (int i = 0; i < g_terrain.GetWidth(); ++i)
	{
		for (int j = 0; j < g_terrain.GetWidth(); ++j)
		{
			//Setting up the node
			Node cur_node;
			cur_node.cost = 0;
			cur_node.given = 0;
			cur_node.my_list = None;

			//Check neighbors and add to the list
			//Check right side
			if (j < g_terrain.GetWidth() - 1)
			{
				//Right
				if (g_terrain.IsWall(i, j + 1) == TILE_EMPTY)
				{
					cur_node.neighbours.push_back(std::make_pair(i, j + 1));

					//Top Right
					if (i > 0)
						if (g_terrain.IsWall(i - 1, j) == TILE_EMPTY && g_terrain.IsWall(i - 1, j + 1) == TILE_EMPTY)
							cur_node.neighbours.push_back(std::make_pair(i - 1, j + 1));

					//Bottom Right
					if (i < g_terrain.GetWidth() - 1)
						if (g_terrain.IsWall(i + 1, j) == TILE_EMPTY && g_terrain.IsWall(i + 1, j + 1) == TILE_EMPTY)
							cur_node.neighbours.push_back(std::make_pair(i + 1, j + 1));
				}
			}

			//Check left side
			if (j > 0)
			{
				//Left
				if (g_terrain.IsWall(i, j - 1) == TILE_EMPTY)
				{
					cur_node.neighbours.push_back(std::make_pair(i, j - 1));

					//Top Left
					if (i > 0)
						if (g_terrain.IsWall(i - 1, j) == TILE_EMPTY && g_terrain.IsWall(i - 1, j - 1) == TILE_EMPTY)
							cur_node.neighbours.push_back(std::make_pair(i - 1, j - 1));

					//Bottom Left
					if (i < g_terrain.GetWidth() - 1)
						if (g_terrain.IsWall(i + 1, j) == TILE_EMPTY && g_terrain.IsWall(i + 1, j - 1) == TILE_EMPTY)
							cur_node.neighbours.push_back(std::make_pair(i + 1, j - 1));
				}
			}

			//Check Top
			if (i > 0 && g_terrain.IsWall(i - 1, j) == TILE_EMPTY)
				cur_node.neighbours.push_back(std::make_pair(i - 1, j));

			//Check Bottom
			if (i < g_terrain.GetWidth() - 1 && g_terrain.IsWall(i + 1, j) == TILE_EMPTY)
				cur_node.neighbours.push_back(std::make_pair(i + 1, j));

			g_map[i][j] = cur_node;
		}
	}
}

bool StraightLine(int start_row, int start_col, int r, int c)
{
	int smaller_row;
	int smaller_col;
	int bigger_row;
	int bigger_col;

	if (start_row >= r)
	{
		smaller_row = r;
		bigger_row = start_row;
	}
	else
	{
		smaller_row = start_row;
		bigger_row = r;
	}

	if (start_col >= c)
	{
		smaller_col = c;
		bigger_col = start_col;
	}
	else
	{
		smaller_col = start_col;
		bigger_col = c;
	}

	//Check if there is a wall
	for (int i = smaller_row; i < bigger_row + 1; ++i)
	{
		for (int j = smaller_col; j < bigger_col + 1; ++j)
		{
			if (g_terrain.IsWall(i, j))
			{
				return false;
			}
		}
	}

	return true;
}

bool Movement::ComputePath( int r, int c, bool newRequest)
{

	m_goal = g_terrain.GetCoordinates( r, c );
	m_movementMode = MOVEMENT_WAYPOINT_LIST;

	// project 2: change this flag to true
	float sq_2 = (float)sqrt(2);
	bool useAStar = true;
	int start_row, start_col;
	D3DXVECTOR3 start_pos = m_owner->GetBody().GetPos();

	g_terrain.GetRowColumn(&start_pos, &start_row, &start_col);


	if (GetStraightlinePath())
	{
		m_waypointList.clear();


		if (StraightLine(start_row, start_col, r, c))
		{
			m_waypointList.push_back(g_terrain.GetCoordinates(r, c));
			return true;
		}
	}

	if (newRequest)
	{
		ZeroNodes();
		m_waypointList.clear();
		g_open_list.clear();

		g_terrain.SetColor(start_row, start_col, DEBUG_COLOR_BLUE);
		//Setting up starting node
		g_map[start_row][start_col].my_list = Open;
		g_map[start_row][start_col].col_parent = start_col;
		g_map[start_row][start_col].row_parent = start_row;

		g_open_list.push_back(std::make_pair(start_row, start_col));
	}


	if( useAStar )
	{
		///////////////////////////////////////////////////////////////////////////////////////////////////
		//INSERT YOUR A* CODE HERE
		//1. You should probably make your own A* class.
		//2. You will need to make this function remember the current progress if it preemptively exits.
		//3. Return "true" if the path is complete, otherwise "false".
		///////////////////////////////////////////////////////////////////////////////////////////////////
		//Loop until list is empty
		while (!g_open_list.empty())
		{
			std::pair<int, int> cur_node_pos = g_open_list.front();

			//Find the least cost node
			for (auto it = g_open_list.begin(); it != g_open_list.end(); ++it)
			{
				if (g_map[it->first][it->second].cost < g_map[cur_node_pos.first][cur_node_pos.second].cost)
					cur_node_pos = (*it);
			}

			g_open_list.remove(cur_node_pos);

			//Check if current node is goal
			if (m_goal == g_terrain.GetCoordinates(cur_node_pos.first, cur_node_pos.second))
			{
				g_terrain.SetColor(r, c, DEBUG_COLOR_YELLOW);

				//Push back entire close list on waypoint list
				while (cur_node_pos.first != start_row || cur_node_pos.second != start_col)
				{
					D3DXVECTOR3 pos = g_terrain.GetCoordinates(cur_node_pos.first, cur_node_pos.second);
					m_waypointList.push_front(pos);

					cur_node_pos = std::make_pair(g_map[cur_node_pos.first][cur_node_pos.second].row_parent,
											      g_map[cur_node_pos.first][cur_node_pos.second].col_parent);
				}

				m_waypointList.push_front(start_pos);

				//Do rubber banding
				if (GetRubberbandPath())
				{
					std::vector<D3DXVECTOR3> temp;

					//Convert it to vector to make this easier
					for (auto it = m_waypointList.begin(); it != m_waypointList.end(); ++it)
						temp.push_back(D3DXVECTOR3(it->x, it->y, it->z));

					//Loop through and check straight lines
					for(int i = temp.size() - 1; i > 1 ; --i)
					{
						int j = i - 2;
						int node1_row;
						int node1_col;
						int node2_row;
						int node2_col;

						g_terrain.GetRowColumn(&temp[i], &node1_row, &node1_col);
						g_terrain.GetRowColumn(&temp[j], &node2_row, &node2_col);

						while (StraightLine(node1_row, node1_col, node2_row, node2_col))
						{
							temp.erase(temp.begin() + i - 1);
							j--;
							i--;

							if (j < 0)
								break;

							g_terrain.GetRowColumn(&temp[j], &node2_row, &node2_col);
						}
					}
					
					//Put it back into m_waypointlist
					m_waypointList.clear();
					for (unsigned i = 0; i < temp.size(); i++)
						m_waypointList.push_back(temp[i]);
				}

				if (GetSmoothPath())
				{
					//Check if rubberbanding and add points back in if line to long
					if (GetRubberbandPath())
					{
						std::vector<D3DXVECTOR3> temp;

						//Convert it to vector to make this easier
						for (auto it = m_waypointList.begin(); it != m_waypointList.end(); ++it)
							temp.push_back(D3DXVECTOR3(it->x, it->y, it->z));

						m_waypointList.clear();

						//Loop through and add points
						for (unsigned i = 0; i < temp.size() - 1; i++)
						{
							int node1_row;
							int node1_col;
							int node2_row;
							int node2_col;

							g_terrain.GetRowColumn(&temp[i], &node1_row, &node1_col);
							g_terrain.GetRowColumn(&temp[i + 1], &node2_row, &node2_col);

							float distance = (float)sqrt((node2_row - node1_row) * (node2_row - node1_row) +
														 (node2_col - node1_col) * (node2_col - node1_col));

							//Loop through till distance less than 1.5
							while (distance > 1.5f)
							{
								D3DXVECTOR3 temp_node = (temp[i] + temp[i + 1]) / 2;

								temp.insert(temp.begin() + i + 1, temp_node);

								g_terrain.GetRowColumn(&temp[i + 1], &node2_row, &node2_col);

								distance = (float)sqrt((node2_row - node1_row) * (node2_row - node1_row) +
													   (node2_col - node1_col) * (node2_col - node1_col));
							}
						}

						for (unsigned i = 0; i < temp.size(); i++)
							m_waypointList.push_back(temp[i]);
					}

					std::vector<D3DXVECTOR3> temp;

					//Convert it to vector to make this easier
					for (auto it = m_waypointList.begin(); it != m_waypointList.end(); ++it)
						temp.push_back(D3DXVECTOR3(it->x, it->y, it->z));

					m_waypointList.clear();

					//Checks to see special cases
					if (temp.size() == 1)
						m_waypointList.push_back(temp[0]);
					else if (temp.size() == 2)
					{
						D3DXVECTOR3 p_out1, p_out2, p_out3;

						D3DXVec3CatmullRom(&p_out1, &temp[0], &temp[0], &temp[1], &temp[1], 0.25f);
						D3DXVec3CatmullRom(&p_out2, &temp[0], &temp[0], &temp[1], &temp[1], 0.5f);
						D3DXVec3CatmullRom(&p_out3, &temp[0], &temp[0], &temp[1], &temp[1], 0.75f);

						m_waypointList.push_back(temp[0]);
						m_waypointList.push_back(p_out1);
						m_waypointList.push_back(p_out2);
						m_waypointList.push_back(p_out3);
						m_waypointList.push_back(temp[1]);

					}
					else if (temp.size() == 3)
					{
						D3DXVECTOR3 p_out1, p_out2, p_out3;

						D3DXVec3CatmullRom(&p_out1, &temp[0], &temp[0], &temp[1], &temp[2], 0.25f);
						D3DXVec3CatmullRom(&p_out2, &temp[0], &temp[0], &temp[1], &temp[2], 0.5f);
						D3DXVec3CatmullRom(&p_out3, &temp[0], &temp[0], &temp[1], &temp[2], 0.75f);

						m_waypointList.push_back(temp[0]);
						m_waypointList.push_back(p_out1);
						m_waypointList.push_back(p_out2);
						m_waypointList.push_back(p_out3);
						m_waypointList.push_back(temp[1]);

						D3DXVec3CatmullRom(&p_out1, &temp[0], &temp[1], &temp[2], &temp[2], 0.25f);
						D3DXVec3CatmullRom(&p_out2, &temp[0], &temp[1], &temp[2], &temp[2], 0.5f);
						D3DXVec3CatmullRom(&p_out3, &temp[0], &temp[1], &temp[2], &temp[2], 0.75f);

						m_waypointList.push_back(p_out1);
						m_waypointList.push_back(p_out2);
						m_waypointList.push_back(p_out3);
						m_waypointList.push_back(temp[2]);

					}
					else
					{
						for (unsigned i = 0; i < temp.size() - 1; ++i)
						{
							D3DXVECTOR3 p_out1, p_out2, p_out3;
							D3DXVECTOR3 p1;
							D3DXVECTOR3 p2;
							D3DXVECTOR3 p3;
							D3DXVECTOR3 p4;

							//First scenario
							if (i == 0)
							{
								m_waypointList.push_back(temp[0]);

								p1 = temp[0];
								p2 = temp[0];
								p3 = temp[1];
								p4 = temp[2];
							}
							else if (i == temp.size() - 2)
							{
								m_waypointList.push_back(temp[i]);
								p1 = temp[i - 1];
								p2 = temp[i];
								p3 = temp[i + 1];
								p4 = temp[i + 1];
							}
							else
							{
								m_waypointList.push_back(temp[i]);
								p1 = temp[i - 1];
								p2 = temp[i];
								p3 = temp[i + 1];
								p4 = temp[i + 2];
							}

							D3DXVec3CatmullRom(&p_out1, &p1, &p2, &p3, &p4, 0.25f);
							D3DXVec3CatmullRom(&p_out2, &p1, &p2, &p3, &p4, 0.5f);
							D3DXVec3CatmullRom(&p_out3, &p1, &p2, &p3, &p4, 0.75f);

							m_waypointList.push_back(p_out1);
							m_waypointList.push_back(p_out2);
							m_waypointList.push_back(p_out3);
						}

						m_waypointList.push_back(temp.back());
					}
				}

				return true;
			}

			//Loop through Children
			for (auto it = g_map[cur_node_pos.first][cur_node_pos.second].neighbours.begin(); it != g_map[cur_node_pos.first][cur_node_pos.second].neighbours.end();it++)
			{
				//Compute cost
				float cost;

				//Manhattan Heuristic
				if (GetHeuristicCalc() == 3)
					cost = static_cast<float>(abs(it->first - r) + abs(it->second - c));
				else if (GetHeuristicCalc() == 2) // Chebyshev Distance
					cost = static_cast<float>(max(abs(it->first - r), abs(it->second - c)));
				else if (GetHeuristicCalc() == 0) // Euclidian Distance
				{
					int xdif = it->first - r;
					int ydif = it->second - c;

					cost = (float)sqrt((xdif * xdif) + (ydif * ydif));
				}
				else if (GetHeuristicCalc() == 1) // Octile Distance
				{
					int xdif = it->first - r;
					int ydif = it->second - c;

					int max_val = max(abs(xdif), abs(ydif));
					int min_val = min(abs(xdif), abs(ydif));

					cost = min_val * sq_2 + max_val - min_val;
				}

				bool diagnol;

				//Check if diagnol neighbor
				if ((it->first == cur_node_pos.first + 1 && it->second == cur_node_pos.second + 1) ||
					(it->first == cur_node_pos.first + 1 && it->second == cur_node_pos.second - 1) ||
					(it->first == cur_node_pos.first - 1 && it->second == cur_node_pos.second + 1) ||
					(it->first == cur_node_pos.first - 1 && it->second == cur_node_pos.second - 1))
				{
					cost = g_map[cur_node_pos.first][cur_node_pos.second].given + sq_2 + (cost * GetHeuristicWeight());
					diagnol = true;
				}
				else
				{
					cost = g_map[cur_node_pos.first][cur_node_pos.second].given + 1.0f + (cost * GetHeuristicWeight());
					diagnol = false;
				}

				//If not on open or closed, then put on open
				if (g_map[it->first][it->second].my_list == None)
				{
					g_map[it->first][it->second].cost = cost;
					g_map[it->first][it->second].my_list = Open;
					g_map[it->first][it->second].col_parent = cur_node_pos.second;
					g_map[it->first][it->second].row_parent = cur_node_pos.first;
					if (diagnol)
						g_map[it->first][it->second].given = g_map[cur_node_pos.first][cur_node_pos.second].given + sq_2;
					else
						g_map[it->first][it->second].given = g_map[cur_node_pos.first][cur_node_pos.second].given + 1.0f;


					g_open_list.push_back(*it);

					g_terrain.SetColor(it->first, it->second, DEBUG_COLOR_BLUE);

					continue;
				}

				//If on open or closed, and new one is cheaper
				if (cost < g_map[it->first][it->second].cost)
				{
					//Replace node on map
					g_map[it->first][it->second].cost = cost;
					g_map[it->first][it->second].my_list = Open;
					g_map[it->first][it->second].col_parent = cur_node_pos.second;
					g_map[it->first][it->second].row_parent = cur_node_pos.first;
					if (diagnol)
						g_map[it->first][it->second].given = g_map[cur_node_pos.first][cur_node_pos.second].given + sq_2;
					else
						g_map[it->first][it->second].given = g_map[cur_node_pos.first][cur_node_pos.second].given + 1.0f;

					//remove coordinates from open and closed
					g_open_list.remove(*it);

					//Put it back on open
					g_open_list.push_back(*it);
				}
			}
			//Place parent node on closed list
			g_map[cur_node_pos.first][cur_node_pos.second].my_list = Close;
			g_terrain.SetColor(cur_node_pos.first, cur_node_pos.second, DEBUG_COLOR_YELLOW);


			if (GetSingleStep())
				return false;

		}

		//If there is no path found it returns fail
		return false;

	}
	else
	{	
		//Randomly meander toward goal (might get stuck at wall)
		int curR, curC;
		D3DXVECTOR3 cur = m_owner->GetBody().GetPos();
		g_terrain.GetRowColumn( &cur, &curR, &curC );

		m_waypointList.clear();
		m_waypointList.push_back( cur );

		int countdown = 100;
		while( curR != r || curC != c )
		{
			if( countdown-- < 0 ) { break; }

			if( curC == c || (curR != r && rand()%2 == 0) )
			{	//Go in row direction
				int last = curR;
				if( curR < r ) { curR++; }
				else { curR--; }

				if( g_terrain.IsWall( curR, curC ) )
				{
					curR = last;
					continue;
				}
			}
			else
			{	//Go in column direction
				int last = curC;
				if( curC < c ) { curC++; }
				else { curC--; }

				if( g_terrain.IsWall( curR, curC ) )
				{
					curC = last;
					continue;
				}
			}

			D3DXVECTOR3 spot = g_terrain.GetCoordinates( curR, curC );
			m_waypointList.push_back( spot );
			g_terrain.SetColor( curR, curC, DEBUG_COLOR_YELLOW );
		}
		return true;
	}
}
