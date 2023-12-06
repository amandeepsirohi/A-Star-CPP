#include <iostream>
#include <bits/stdc++.h>
#include <vector>
#include <string>
using namespace std;

#define ROW 14
#define COL 14

// shortcut for pair<int,int>
typedef pair<int, int> Pair;

// shortcut for pair<int,pair<int,int>> type
typedef pair<double, pair<int, int>> pPair;

// strcut to hold row , col , heuristics as parameters
struct cell
{
    // Row and Col index of its parent
    // 0 <= i <= ROW-1 && 0 <= j <= COL -1
    int parent_i, parent_j;

    // final distance(f) = heuristic(h) + distance(cost) to go to next cell(g)
    int f, g, h;
};

// check validity of cell
bool isCellValid(int row, int col)
{
    return (row >= 0) && (row < ROW) && (col >= 0) && (col < COL);
}

// check for next cell is blocked/unblocked
bool isUnblocked(int grid[][COL], int row, int col)
{
    if (grid[row][col])
        return true;
    return false;
}

// check for destination
bool isFinalDestination(int row, int col, Pair dest)
{
    if (row == dest.first && col == dest.second)
        return true;
    return false;
}

// calc distance(heuristics) Change this fxn accordingly
double calcHVal(int row, int col, Pair dest)
{
    return ((double)sqrt((row - dest.first) * (row - dest.first) +
                         (col - dest.second) * (col - dest.second)));
}

// trace the path
void traceFinalPath(cell cellInformation[][COL], Pair dest, int grid[][COL])
{
    std::cout << "The final path is ";
    int row = dest.first;
    int col = dest.second;
    stack<Pair> Path;

    

    while (!(cellInformation[row][col].parent_i == row &&
             cellInformation[row][col].parent_j == col))
    {
        Path.push({row, col});
        int temp_row = cellInformation[row][col].parent_i;
        int temp_col = cellInformation[row][col].parent_j;
        row = temp_row;
        col = temp_col;
    }

    Path.push({row, col});

    

    while (!Path.empty())
    {
        Pair p = Path.top();
        Path.pop();
        std::cout << "->"
                  << "(" << p.first << "," << p.second << ")";
        grid[p.first][p.second] = 8;
    }
    std::cout << std::endl;
    // for (int i = 0; i < ROW; i++)
    // {
    //     for (int j = 0; j < COL; j++)
    //     {
    //         if (grid[i][j] != 8)
    //             grid[i][j] = 0;
    //         else if (grid[i][j] == 8)
    //             grid[i][j] = 2;
    //     }
    // }
    std::cout << std::endl;
    std::cout << "The shortest path is represented by 8's , 1's are unblocked cells and 0's are blocked cells" << std::endl;
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }
    return;
}

// Implementing main fxn(A*)
void aStarSearch(int grid[][COL], Pair src, Pair dest)
{
    // checks if source is in given constraints
    if (!isCellValid(src.first, src.second))
    {
        std::cout << "Invalid Source " << endl;
        return;
    }

    // checks if dest is in given constraints
    if (!isCellValid(dest.first, dest.second))
    {
        std::cout << "Invalid Destination " << endl;
        return;
    }

    // check if src is blocked
    if (!isUnblocked(grid, src.first, src.second))
    {
        std::cout << "Source is blocked" << endl;
        return;
    }

    // check if dest is blocked
    if (!isUnblocked(grid, dest.first, dest.second))
    {
        std::cout << "Destination is blocked" << endl;
        return;
    }

    // check if src and dest are same
    if (isFinalDestination(src.first, src.second, dest))
    {
        std::cout << "source and destination are same " << endl;
        return;
    }

    // create an array to check if a cell is visited
    bool closedList[ROW][COL];
    memset(closedList, false, sizeof(closedList));

    // create an array of struct to hold details of the particular cell
    cell cellInformation[ROW][COL];

    int i, j;
    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COL; j++)
        {
            cellInformation[i][j].f = INT_MAX;
            cellInformation[i][j].g = INT_MAX;
            cellInformation[i][j].h = INT_MAX;
            cellInformation[i][j].parent_i = -1;
            cellInformation[i][j].parent_j = -1;
        }
    }

    // set parameters for starting node
    i = src.first, j = src.second;
    cellInformation[i][j].f = 0.0;
    cellInformation[i][j].g = 0.0;
    cellInformation[i][j].h = 0.0;
    cellInformation[i][j].parent_i = i;
    cellInformation[i][j].parent_j = j;

    /*Create a container having f , g , h as <f <g , h>>
        where f = g + h , implement using Pair of Pairs
    */
    set<pPair> openList;

    // Enter starting point to list
    openList.insert({0.0, {i, j}});

    bool findDest = false;

    while (!openList.empty())
    {
        pPair p = *openList.begin();

        // Remove vertex from openList
        openList.erase(openList.begin());

        // Add this vertex to closedList
        i = p.second.first;
        j = p.second.second;
        closedList[i][j] = true;
        /*

                Generating all possible movements of this cell

                ↖ (r-1, c-1) | ↑ (r-1, c) | ↗ (r-1, c+1)
                ---------------------------------------
                ← (r, c-1)    | (r, c)     | → (r, c+1)
                ---------------------------------------
                ↙ (r+1, c-1) | ↓ (r+1, c) | ↘ (r+1, c+1)

                Cell-->Removed pair (r, c)
                In this diagram:

                The arrows represent the direction of movement.
                The numbers 0 to 7 correspond to the 8 possible directions:
                0: UP (r-1, c)
                1: UP-RIGHT (r-1, c+1)
                2: RIGHT (r, c+1)
                3: DOWN-RIGHT (r+1, c+1)
                4: DOWN (r+1, c)
                5: DOWN-LEFT (r+1, c-1)
                6: LEFT (r, c-1)
                7: UP-LEFT (r-1, c-1)

        */

        // To store the 'g', 'h' and 'f' of the 8 successors
        double gNew, hNew, fNew;

        // 1St successor -------------------->(UP)
        // check if cell is a valid cell or not
        if (isCellValid(i - 1, j))
        {
            // check is dest is same as current cell
            if (isFinalDestination(i - 1, j, dest))
            {
                // set parents for destination cell
                cellInformation[i - 1][j].parent_i = i;
                cellInformation[i - 1][j].parent_j = j;
                std::cout << "The destination cell is found " << std::endl;
                traceFinalPath(cellInformation, dest, grid);
                findDest = true;
                return;
            }
            // If the successor already in list or it is blocked
            else if (closedList[i - 1][j] == false && isUnblocked(grid, i - 1, j))
            {
                gNew = cellInformation[i][j].g + 1.0;
                hNew = calcHVal(i - 1, j, dest);
                fNew = gNew + hNew;

                // Check if the cell is in the open list if not then add or if better value get then update
                if (cellInformation[i - 1][j].f == FLT_MAX || cellInformation[i - 1][j].f > fNew)
                {
                    openList.insert({fNew, {i - 1, j}});

                    // update details of cell
                    cellInformation[i - 1][j].parent_i = i;
                    cellInformation[i - 1][j].parent_j = j;
                    cellInformation[i - 1][j].f = fNew;
                    cellInformation[i - 1][j].g = gNew;
                    cellInformation[i - 1][j].h = hNew;
                }
            }
        }
        // 2nd successor -------------------->(UP-RIGHT)
        // check if cell is a valid cell or not
        if (isCellValid(i + 1, j))
        {
            // check is dest is same as current cell
            if (isFinalDestination(i + 1, j, dest))
            {
                // set parents for destination cell
                cellInformation[i + 1][j].parent_i = i;
                cellInformation[i + 1][j].parent_j = j;
                std::cout << "The destination cell is found " << std::endl;
                traceFinalPath(cellInformation, dest, grid);
                findDest = true;
                return;
            }
            // If the successor already in list or it is blocked
            else if (closedList[i + 1][j] == false && isUnblocked(grid, i + 1, j))
            {
                gNew = cellInformation[i][j].g + 1.0;
                hNew = calcHVal(i + 1, j, dest);
                fNew = gNew + hNew;

                // Check if the cell is in the open list if not then add or if better value get then update
                if (cellInformation[i + 1][j].f == FLT_MAX || cellInformation[i + 1][j].f > fNew)
                {
                    openList.insert({fNew, {i + 1, j}});

                    // update details of cell
                    cellInformation[i + 1][j].parent_i = i;
                    cellInformation[i + 1][j].parent_j = j;
                    cellInformation[i + 1][j].f = fNew;
                    cellInformation[i + 1][j].g = gNew;
                    cellInformation[i + 1][j].h = hNew;
                }
            }
        }
        // 3rd successor -------------------->(DOWN)
        // check if cell is a valid cell or not
        if (isCellValid(i, j + 1))
        {
            // check is dest is same as current cell
            if (isFinalDestination(i, j + 1, dest))
            {
                // set parents for destination cell
                cellInformation[i][j + 1].parent_i = i;
                cellInformation[i][j + 1].parent_j = j;
                std::cout << "The destination cell is found " << std::endl;
                traceFinalPath(cellInformation, dest, grid);
                findDest = true;
                return;
            }
            // If the successor already in list or it is blocked
            else if (closedList[i][j + 1] == false && isUnblocked(grid, i, j + 1))
            {
                gNew = cellInformation[i][j].g + 1.0;
                hNew = calcHVal(i, j + 1, dest);
                fNew = gNew + hNew;

                // Check if the cell is in the open list if not then add or if better value get then update
                if (cellInformation[i][j + 1].f == FLT_MAX || cellInformation[i][j + 1].f > fNew)
                {
                    openList.insert({fNew, {i, j + 1}});

                    // update details of cell
                    cellInformation[i][j + 1].parent_i = i;
                    cellInformation[i][j + 1].parent_j = j;
                    cellInformation[i][j + 1].f = fNew;
                    cellInformation[i][j + 1].g = gNew;
                    cellInformation[i][j + 1].h = hNew;
                }
            }
        }
        // 4th successor -------------------->(DOWN-RIGHT)
        // check if cell is a valid cell or not
        if (isCellValid(i, j - 1))
        {
            // check is dest is same as current cell
            if (isFinalDestination(i, j - 1, dest))
            {
                // set parents for destination cell
                cellInformation[i][j - 1].parent_i = i;
                cellInformation[i][j - 1].parent_j = j;
                std::cout << "The destination cell is found " << std::endl;
                traceFinalPath(cellInformation, dest, grid);
                findDest = true;
                return;
            }
            // If the successor already in list or it is blocked
            else if (closedList[i][j - 1] == false && isUnblocked(grid, i, j - 1))
            {
                gNew = cellInformation[i][j].g + 1.0;
                hNew = calcHVal(i, j - 1, dest);
                fNew = gNew + hNew;

                // Check if the cell is in the open list if not then add or if better value get then update
                if (cellInformation[i][j - 1].f == FLT_MAX || cellInformation[i][j - 1].f > fNew)
                {
                    openList.insert({fNew, {i, j - 1}});

                    // update details of cell
                    cellInformation[i][j - 1].parent_i = i;
                    cellInformation[i][j - 1].parent_j = j;
                    cellInformation[i][j - 1].f = fNew;
                    cellInformation[i][j - 1].g = gNew;
                    cellInformation[i][j - 1].h = hNew;
                }
            }
        }
        // 5th successor -------------------->(DOWN)
        // check if cell is a valid cell or not
        if (isCellValid(i - 1, j + 1))
        {
            // check is dest is same as current cell
            if (isFinalDestination(i - 1, j + 1, dest))
            {
                // set parents for destination cell
                cellInformation[i - 1][j + 1].parent_i = i;
                cellInformation[i - 1][j + 1].parent_j = j;
                std::cout << "The destination cell is found " << std::endl;
                traceFinalPath(cellInformation, dest, grid);
                findDest = true;
                return;
            }
            // If the successor already in list or it is blocked
            else if (closedList[i - 1][j + 1] == false && isUnblocked(grid, i - 1, j + 1))
            {
                gNew = cellInformation[i][j].g + 1.414;
                hNew = calcHVal(i - 1, j + 1, dest);
                fNew = gNew + hNew;

                // Check if the cell is in the open list if not then add or if better value get then update
                if (cellInformation[i - 1][j + 1].f == FLT_MAX || cellInformation[i - 1][j + 1].f > fNew)
                {
                    openList.insert({fNew, {i - 1, j + 1}});

                    // update details of cell
                    cellInformation[i - 1][j + 1].parent_i = i;
                    cellInformation[i - 1][j + 1].parent_j = j;
                    cellInformation[i - 1][j + 1].f = fNew;
                    cellInformation[i - 1][j + 1].g = gNew;
                    cellInformation[i - 1][j + 1].h = hNew;
                }
            }
        }
        // 6th successor -------------------->(DOWN-LEFT)
        // check if cell is a valid cell or not
        if (isCellValid(i - 1, j - 1))
        {
            // check is dest is same as current cell
            if (isFinalDestination(i - 1, j + 1, dest))
            {
                // set parents for destination cell
                cellInformation[i - 1][j - 1].parent_i = i;
                cellInformation[i - 1][j - 1].parent_j = j;
                std::cout << "The destination cell is found " << std::endl;
                traceFinalPath(cellInformation, dest, grid);
                findDest = true;
                return;
            }
            // If the successor already in list or it is blocked
            else if (closedList[i - 1][j - 1] == false && isUnblocked(grid, i - 1, j - 1))
            {
                gNew = cellInformation[i][j].g + 1.414;
                hNew = calcHVal(i - 1, j - 1, dest);
                fNew = gNew + hNew;

                // Check if the cell is in the open list if not then add or if better value get then update
                if (cellInformation[i - 1][j - 1].f == FLT_MAX || cellInformation[i - 1][j - 1].f > fNew)
                {
                    openList.insert({fNew, {i - 1, j - 1}});

                    // update details of cell
                    cellInformation[i - 1][j - 1].parent_i = i;
                    cellInformation[i - 1][j - 1].parent_j = j;
                    cellInformation[i - 1][j - 1].f = fNew;
                    cellInformation[i - 1][j - 1].g = gNew;
                    cellInformation[i - 1][j - 1].h = hNew;
                }
            }
        }
        // 7th successor -------------------->(LEFT)
        // check if cell is a valid cell or not
        if (isCellValid(i + 1, j + 1))
        {
            // check is dest is same as current cell
            if (isFinalDestination(i + 1, j + 1, dest))
            {
                // set parents for destination cell
                cellInformation[i + 1][j + 1].parent_i = i;
                cellInformation[i + 1][j + 1].parent_j = j;
                std::cout << "The destination cell is found " << std::endl;
                traceFinalPath(cellInformation, dest, grid);
                findDest = true;
                return;
            }
            // If the successor already in list or it is blocked
            else if (closedList[i + 1][j + 1] == false && isUnblocked(grid, i + 1, j + 1))
            {
                gNew = cellInformation[i][j].g + 1.414;
                hNew = calcHVal(i + 1, j + 1, dest);
                fNew = gNew + hNew;

                // Check if the cell is in the open list if not then add or if better value get then update
                if (cellInformation[i + 1][j + 1].f == FLT_MAX || cellInformation[i + 1][j + 1].f > fNew)
                {
                    openList.insert({fNew, {i + 1, j + 1}});

                    // update details of cell
                    cellInformation[i + 1][j + 1].parent_i = i;
                    cellInformation[i + 1][j + 1].parent_j = j;
                    cellInformation[i + 1][j + 1].f = fNew;
                    cellInformation[i + 1][j + 1].g = gNew;
                    cellInformation[i + 1][j + 1].h = hNew;
                }
            }
        }
        // 8th successor -------------------->(UP-LEFT)
        // check if cell is a valid cell or not
        if (isCellValid(i + 1, j - 1))
        {
            // check is dest is same as current cell
            if (isFinalDestination(i + 1, j - 1, dest))
            {
                // set parents for destination cell
                cellInformation[i + 1][j - 1].parent_i = i;
                cellInformation[i + 1][j - 1].parent_j = j;
                std::cout << "The destination cell is found " << std::endl;
                traceFinalPath(cellInformation, dest, grid);
                findDest = true;
                return;
            }
            // If the successor already in list or it is blocked
            else if (closedList[i + 1][j - 1] == false && isUnblocked(grid, i + 1, j - 1))
            {
                gNew = cellInformation[i][j].g + 1.414;
                hNew = calcHVal(i + 1, j - 1, dest);
                fNew = gNew + hNew;

                // Check if the cell is in the open list if not then add or if better value get then update
                if (cellInformation[i + 1][j - 1].f == FLT_MAX || cellInformation[i + 1][j - 1].f > fNew)
                {
                    openList.insert({fNew, {i + 1, j - 1}});

                    // update details of cell
                    cellInformation[i + 1][j - 1].parent_i = i;
                    cellInformation[i + 1][j - 1].parent_j = j;
                    cellInformation[i + 1][j - 1].f = fNew;
                    cellInformation[i + 1][j - 1].g = gNew;
                    cellInformation[i + 1][j - 1].h = hNew;
                }
            }
        }
    }
    // if teh dest cell is ot found due to any factors return false
    if (findDest == false)
    {
        std::cout << "The destination cell is not found " << std::endl;
    }
    return;
}
int main()
{

    /* 1--> The cell is not blocked
     0--> The cell is blocked    */
    int grid[ROW][COL] = {{1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1},
                          {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
                          {1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1},
                          {0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1},
                          {1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1},
                          {1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1},
                          {1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1},
                          {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1},
                          {1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1},
                          {1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1},
                          {0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1},
                          {1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1},
                          {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
                          {1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1}};

    Pair src = {12, 13};

    Pair dest = {2, 4};

    aStarSearch(grid, src, dest);

    return (0);
}
