#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct TCell
{
  struct TCell             * m_Right;
  struct TCell             * m_Down;
  int                        m_Row;
  int                        m_Col;
  int                        m_Data;
} TCELL;

typedef struct TRowCol
{
  struct TRowCol           * m_Next;
  TCELL                    * m_Cells;
  int                        m_Idx;
} TROWCOL;

typedef struct TSparseMatrix
{
  TROWCOL                  * m_Rows;
  TROWCOL                  * m_Cols;
} TSPARSEMATRIX;
#endif /* __PROGTEST__ */

void initMatrix   ( TSPARSEMATRIX   * m )
{
  m->m_Rows = nullptr;
  m->m_Cols = nullptr;
}
/**
*@arg : prev, curr
*/
bool find_TROWCOL(TROWCOL **curr, TROWCOL **prev, int rowColIdx)
{      
    while(*curr && (*curr)->m_Idx <= rowColIdx)
    {   
        if((*curr)->m_Idx == rowColIdx)
        {
            return true;
        } 
        *prev = *curr;
        *curr = (*curr)->m_Next;
    }  
    return false;
}
/**
*@arg  curr : TCELL with the same colIdx/ bigger colIdx, prev : TCELL with smaller colIdx
*/
bool find_TCELL1(TCELL **curr, TCELL **prev, int colIdx)
{
    while(*curr && (*curr)->m_Col <= colIdx)
    {
        if((*curr)->m_Col == colIdx)
        {
            return true;
        }
        *prev = *curr;
        *curr = (*curr)->m_Right;
    }
    return false;
}

bool find_TCELL2(TCELL **curr, TCELL **prev, int rowIdx)
{
    while(*curr && (*curr)->m_Row <= rowIdx)
    {
        if((*curr)->m_Row == rowIdx)
        {
            return true;
        }
        *prev = *curr;
        *curr = (*curr)->m_Down;
    }
    return false;
}

TROWCOL* create_TROWCOL1(TSPARSEMATRIX *m, TROWCOL *prev, TROWCOL *curr, int rowColIdx)
{
    TROWCOL * newlable = (TROWCOL *)malloc(sizeof(TROWCOL));
    newlable->m_Next = curr;
    if(!prev)
        m->m_Rows = newlable; 
    else
        prev->m_Next = newlable;
    newlable->m_Idx = rowColIdx;
    newlable->m_Cells = nullptr;

    return  newlable;
} 
TROWCOL* create_TROWCOL2(TSPARSEMATRIX *m, TROWCOL *prev, TROWCOL *curr, int rowColIdx)
{
    TROWCOL * newlable = (TROWCOL *)malloc(sizeof(TROWCOL));
    newlable->m_Next = curr;
    if(!prev)
        m->m_Cols = newlable; 
    else
        prev->m_Next = newlable;
    newlable->m_Idx = rowColIdx;
    newlable->m_Cells = nullptr;

    return  newlable;
} 

//gotta modify the poointer here
TCELL* create_TCELL(TROWCOL *row, TROWCOL *col, TCELL * left, TCELL * right, TCELL* up, TCELL* down)
{
    TCELL * newCell = (TCELL *)malloc(sizeof(TCELL));
    newCell->m_Right = right;
    newCell->m_Down = down;
    if(!left)
        row->m_Cells = newCell;//row being the TROWCOL
    else
        left->m_Right = newCell;
    if(!up)
        col->m_Cells = newCell;//col being the TROWCOL
    else 
        up->m_Down = newCell;

    return newCell;

}
void modify_TCELL(TCELL ** cell, int rowIdx, int colIdx, int data)
{
    (*cell)->m_Row = rowIdx; 
    (*cell)->m_Col = colIdx;  
    (*cell)->m_Data = data;
}

void addSetCell   ( TSPARSEMATRIX   * m,
                    int               rowIdx,
                    int               colIdx,
                    int               data )
{
    // printf("row%d, col%d\n", rowIdx, colIdx);
    TROWCOL *curr_row = m->m_Rows;
    TROWCOL *prev_row = nullptr;
    bool row = find_TROWCOL(&curr_row, &prev_row, rowIdx);
    TROWCOL *curr_col = m->m_Cols;
    TROWCOL *prev_col = nullptr;
    bool col = find_TROWCOL(&curr_col, &prev_col, colIdx);
    TCELL *thisCell = nullptr;
    if(!(row || col))//both not found
    {  
        //create a new cell, row and colum
        curr_row = create_TROWCOL1(m, prev_row, curr_row, rowIdx);
        curr_col = create_TROWCOL2(m, prev_col, curr_col, colIdx);
        thisCell = create_TCELL(curr_row, curr_col, nullptr , curr_row->m_Cells, nullptr, curr_col->m_Cells);
        modify_TCELL(&thisCell, rowIdx, colIdx, data);
    }
    else if(row && !col)//create a new Colum and a new TCELL
    {
        curr_col = create_TROWCOL2(m, prev_col, curr_col, colIdx);
        TCELL *curr_cell_right = curr_row->m_Cells;
        TCELL *prev_cell_left = nullptr;
        find_TCELL1(&curr_cell_right, &prev_cell_left, colIdx);
        TCELL *curr_cell_down = curr_col->m_Cells;
        TCELL *prev_cell_up = nullptr;
        find_TCELL2(&curr_cell_down, &prev_cell_up, rowIdx);
        thisCell = create_TCELL(curr_row, curr_col, prev_cell_left, curr_cell_right, prev_cell_up, curr_cell_down);
        modify_TCELL(&thisCell, rowIdx, colIdx, data);
    }
    else if(col && !row)//create a new Row and a new TCELL
    {
        curr_row = create_TROWCOL1(m, prev_row, curr_row, rowIdx);
        TCELL *curr_cell_right = curr_row->m_Cells;
        TCELL *prev_cell_left = nullptr;
        find_TCELL1(&curr_cell_right, &prev_cell_left, colIdx);
        TCELL *curr_cell_down = curr_col->m_Cells;
        TCELL *prev_cell_up = nullptr;
        find_TCELL2(&curr_cell_down, &prev_cell_up, rowIdx);
        thisCell = create_TCELL(curr_row, curr_col, prev_cell_left, curr_cell_right, prev_cell_up, curr_cell_down);
        modify_TCELL(&thisCell, rowIdx, colIdx, data);

    }
    else //just create a new TCELL and store data
    {
        TCELL *curr_cell_right = curr_row->m_Cells;
        TCELL *prev_cell_left = nullptr;
        TCELL *curr_cell_down = curr_col->m_Cells;
        TCELL *prev_cell_up = nullptr;
        if(!(find_TCELL1(&curr_cell_right, &prev_cell_left, colIdx)))//★right hand of && wont be done if left handside is false
        {   
            find_TCELL2(&curr_cell_down, &prev_cell_up, rowIdx);
            thisCell = create_TCELL(curr_row, curr_col, prev_cell_left, curr_cell_right, prev_cell_up, curr_cell_down);
    
        }
        //if either one of findTCELL is true -> JUST UPDATE
        else
        {   
            find_TCELL2(&curr_cell_down, &prev_cell_up, rowIdx);
            thisCell = curr_cell_right;
        }
        modify_TCELL(&thisCell, rowIdx, colIdx, data);
    }

}
bool removeCell   ( TSPARSEMATRIX   * m,
                    int               rowIdx,
                    int               colIdx )
{
    
    TROWCOL *curr_row = m->m_Rows, *prev_row = nullptr;
    TROWCOL *curr_col = m->m_Cols, *prev_col = nullptr;
    bool row = find_TROWCOL(&curr_row, &prev_row, rowIdx);
    bool col = find_TROWCOL(&curr_col, &prev_col, colIdx);
    if(!(row && col) )
    {
        return false;
    }
    else
    {
        TCELL *curr_cell_right = curr_row->m_Cells;
        TCELL *prev_cell_left = nullptr;
        if(!find_TCELL1(&curr_cell_right, &prev_cell_left, colIdx))//if CELL dones ont exist
        {
            return false;
        }
        else//it exsict
        {

            TCELL *curr_cell_down = curr_col->m_Cells;
            TCELL *prev_cell_up = nullptr;
            find_TCELL2(&curr_cell_down, &prev_cell_up, rowIdx);
            TCELL *thisCell = curr_cell_down;
            //delete TCELL
            //a) update the row pointer
            if(prev_cell_left)
                prev_cell_left->m_Right = curr_cell_right->m_Right;
            else
               curr_row->m_Cells = curr_cell_right->m_Right;
            //b) update the colum pointer
            if(prev_cell_up)
                prev_cell_up->m_Down = curr_cell_down->m_Down;
            else
                curr_col->m_Cells = curr_cell_down->m_Down;
            //c) free the cell
            free(thisCell);

            //delete the TROWCOL
            //a) checks if the TROWCOL struct points to null(means last element)
            if(!curr_row->m_Cells)
            {
                if(prev_row)
                    prev_row->m_Next = curr_row->m_Next;
                else
                    m->m_Rows = curr_row->m_Next;
                free(curr_row);
            }
            if(!curr_col->m_Cells)
            {
                if(prev_col)
                    prev_col->m_Next = curr_col->m_Next;
                else
                    m->m_Cols = curr_col->m_Next;
                free(curr_col);
            }
        
            return true;
        }
    }

}
void freeMatrix   ( TSPARSEMATRIX   * m )
{
//call removecell function
  TROWCOL *currentRow = m->m_Rows;
  while(currentRow)
  {
    TROWCOL* tmp = currentRow->m_Next;
    TCELL *currentCell = currentRow->m_Cells;
    while(currentCell)
    {
      TCELL *tmp = currentCell->m_Right;
      removeCell(m, currentCell->m_Row, currentCell->m_Col);
      currentCell = tmp;
    }
    currentRow = tmp;
  }
  m = nullptr;
}

bool printMatrix(TSPARSEMATRIX * m){
  TROWCOL * tmp;
  tmp = m -> m_Rows;
  while ( tmp ){
    TCELL * tmpCell = tmp->m_Cells;  
    while ( tmpCell ){
      printf("%d [%d - %d] " , tmpCell->m_Data, tmpCell->m_Row, tmpCell->m_Col);
      if ( tmpCell -> m_Right){
        printf("RIGHT = %d ", tmpCell -> m_Right-> m_Data);
      }else{
        printf("RIGHT = NULL "); 
      }
      if ( tmpCell -> m_Down){
        printf("DOWN = %d ", tmpCell -> m_Down-> m_Data);
      }else{
        printf("DOWN = NULL "); 
      }
      tmpCell = tmpCell->m_Right;
    }
    printf("\n------\n");
    tmp = tmp->m_Next;
  }
  printf("finished printing matrix\n");
  return true;
}
bool printMatrix2(TSPARSEMATRIX * m){
  printf("---------------------\n");
  TROWCOL * tmp;
  tmp = m -> m_Cols;
  while ( tmp ){
    TCELL * tmpCell = tmp->m_Cells;  
    while ( tmpCell ){
      printf("%d [%d - %d] " , tmpCell->m_Data, tmpCell->m_Row, tmpCell->m_Col);
      if ( tmpCell -> m_Right){
        printf("RIGHT = %d ", tmpCell -> m_Right-> m_Data);
      }else{
        printf("RIGHT = NULL "); 
      }
      if ( tmpCell -> m_Down){
        printf("DOWN = %d ", tmpCell -> m_Down-> m_Data);
      }else{
        printf("DOWN = NULL "); 
      }
      tmpCell = tmpCell->m_Down;
    }
    printf("\n------\n");
    tmp = tmp->m_Next;
  }
  printf("finished printing matrix 2\n");
  return true;
}
#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
  TSPARSEMATRIX  m;
  initMatrix ( &m );
  addSetCell ( &m, 0, 1, 10 );
  addSetCell ( &m, 1, 0, 20 );
  addSetCell ( &m, 1, 5, 30 );
  addSetCell ( &m, 2, 1, 40 );

  assert ( m . m_Rows
           && m . m_Rows -> m_Idx == 0
           && m . m_Rows -> m_Cells
           && m . m_Rows -> m_Cells -> m_Row == 0
           && m . m_Rows -> m_Cells -> m_Col == 1
           && m . m_Rows -> m_Cells -> m_Data == 10
           && m . m_Rows -> m_Cells -> m_Right == nullptr );
  assert ( m . m_Rows -> m_Next
           && m . m_Rows -> m_Next -> m_Idx == 1
           && m . m_Rows -> m_Next -> m_Cells
           && m . m_Rows -> m_Next -> m_Cells -> m_Row == 1
           && m . m_Rows -> m_Next -> m_Cells -> m_Col == 0
           && m . m_Rows -> m_Next -> m_Cells -> m_Data == 20
           && m . m_Rows -> m_Next -> m_Cells -> m_Right
           && m . m_Rows -> m_Next -> m_Cells -> m_Right -> m_Row == 1
           && m . m_Rows -> m_Next -> m_Cells -> m_Right -> m_Col == 5
           && m . m_Rows -> m_Next -> m_Cells -> m_Right -> m_Data == 30
           && m . m_Rows -> m_Next -> m_Cells -> m_Right -> m_Right == nullptr );
  assert ( m . m_Rows -> m_Next -> m_Next
           && m . m_Rows -> m_Next -> m_Next -> m_Idx == 2
           && m . m_Rows -> m_Next -> m_Next -> m_Cells
           && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Row == 2
           && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Col == 1
           && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Data == 40
           && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Right == nullptr );
  assert ( m . m_Rows -> m_Next -> m_Next -> m_Next == nullptr );
  assert ( m . m_Cols
           && m . m_Cols -> m_Idx == 0
           && m . m_Cols -> m_Cells
           && m . m_Cols -> m_Cells -> m_Row == 1
           && m . m_Cols -> m_Cells -> m_Col == 0
           && m . m_Cols -> m_Cells -> m_Data == 20
           && m . m_Cols -> m_Cells -> m_Down == nullptr );
  assert ( m . m_Cols -> m_Next
           && m . m_Cols -> m_Next -> m_Idx == 1
           && m . m_Cols -> m_Next -> m_Cells
           && m . m_Cols -> m_Next -> m_Cells -> m_Row == 0
           && m . m_Cols -> m_Next -> m_Cells -> m_Col == 1
           && m . m_Cols -> m_Next -> m_Cells -> m_Data == 10
           && m . m_Cols -> m_Next -> m_Cells -> m_Down
           && m . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Row == 2
           && m . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Col == 1
           && m . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Data == 40
           && m . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Down == nullptr );
  assert ( m . m_Cols -> m_Next -> m_Next
           && m . m_Cols -> m_Next -> m_Next -> m_Idx == 5
           && m . m_Cols -> m_Next -> m_Next -> m_Cells
           && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Row == 1
           && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Col == 5
           && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Data == 30
           && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Down == nullptr );
  assert ( m . m_Cols -> m_Next -> m_Next -> m_Next == nullptr );
  assert ( m . m_Rows -> m_Cells == m . m_Cols -> m_Next -> m_Cells );
  assert ( m . m_Rows -> m_Next -> m_Cells == m . m_Cols -> m_Cells );
  assert ( m . m_Rows -> m_Next -> m_Cells -> m_Right == m . m_Cols -> m_Next -> m_Next -> m_Cells );
  assert ( m . m_Rows -> m_Next -> m_Next -> m_Cells == m . m_Cols -> m_Next -> m_Cells -> m_Down );
  addSetCell ( &m, 230, 190, 50 );
  assert ( m . m_Rows
           && m . m_Rows -> m_Idx == 0
           && m . m_Rows -> m_Cells
           && m . m_Rows -> m_Cells -> m_Row == 0
           && m . m_Rows -> m_Cells -> m_Col == 1
           && m . m_Rows -> m_Cells -> m_Data == 10
           && m . m_Rows -> m_Cells -> m_Right == nullptr );
  assert ( m . m_Rows -> m_Next
           && m . m_Rows -> m_Next -> m_Idx == 1
           && m . m_Rows -> m_Next -> m_Cells
           && m . m_Rows -> m_Next -> m_Cells -> m_Row == 1
           && m . m_Rows -> m_Next -> m_Cells -> m_Col == 0
           && m . m_Rows -> m_Next -> m_Cells -> m_Data == 20
           && m . m_Rows -> m_Next -> m_Cells -> m_Right
           && m . m_Rows -> m_Next -> m_Cells -> m_Right -> m_Row == 1
           && m . m_Rows -> m_Next -> m_Cells -> m_Right -> m_Col == 5
           && m . m_Rows -> m_Next -> m_Cells -> m_Right -> m_Data == 30
           && m . m_Rows -> m_Next -> m_Cells -> m_Right -> m_Right == nullptr );
  assert ( m . m_Rows -> m_Next -> m_Next
           && m . m_Rows -> m_Next -> m_Next -> m_Idx == 2
           && m . m_Rows -> m_Next -> m_Next -> m_Cells
           && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Row == 2
           && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Col == 1
           && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Data == 40
           && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Right == nullptr );
  assert ( m . m_Rows -> m_Next -> m_Next -> m_Next
           && m . m_Rows -> m_Next -> m_Next -> m_Next -> m_Idx == 230
           && m . m_Rows -> m_Next -> m_Next -> m_Next -> m_Cells
           && m . m_Rows -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Row == 230
           && m . m_Rows -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Col == 190
           && m . m_Rows -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Data == 50
           && m . m_Rows -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Right == nullptr );
  assert ( m . m_Rows -> m_Next -> m_Next -> m_Next -> m_Next == nullptr );
  assert ( m . m_Cols
           && m . m_Cols -> m_Idx == 0
           && m . m_Cols -> m_Cells
           && m . m_Cols -> m_Cells -> m_Row == 1
           && m . m_Cols -> m_Cells -> m_Col == 0
           && m . m_Cols -> m_Cells -> m_Data == 20
           && m . m_Cols -> m_Cells -> m_Down == nullptr );
  assert ( m . m_Cols -> m_Next
           && m . m_Cols -> m_Next -> m_Idx == 1
           && m . m_Cols -> m_Next -> m_Cells
           && m . m_Cols -> m_Next -> m_Cells -> m_Row == 0
           && m . m_Cols -> m_Next -> m_Cells -> m_Col == 1
           && m . m_Cols -> m_Next -> m_Cells -> m_Data == 10
           && m . m_Cols -> m_Next -> m_Cells -> m_Down
           && m . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Row == 2
           && m . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Col == 1
           && m . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Data == 40
           && m . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Down == nullptr );
  assert ( m . m_Cols -> m_Next -> m_Next
           && m . m_Cols -> m_Next -> m_Next -> m_Idx == 5
           && m . m_Cols -> m_Next -> m_Next -> m_Cells
           && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Row == 1
           && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Col == 5
           && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Data == 30
           && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Down == nullptr );
  assert ( m . m_Cols -> m_Next -> m_Next -> m_Next
           && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Idx == 190
           && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells
           && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Row == 230
           && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Col == 190
           && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Data == 50
           && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Down == nullptr );
  assert ( m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Next == nullptr );
  assert ( m . m_Rows -> m_Cells == m . m_Cols -> m_Next -> m_Cells );
  assert ( m . m_Rows -> m_Next -> m_Cells == m . m_Cols -> m_Cells );
  assert ( m . m_Rows -> m_Next -> m_Cells -> m_Right == m . m_Cols -> m_Next -> m_Next -> m_Cells );
  assert ( m . m_Rows -> m_Next -> m_Next -> m_Cells == m . m_Cols -> m_Next -> m_Cells -> m_Down );
  assert ( m . m_Rows -> m_Next -> m_Next -> m_Next -> m_Cells == m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells );




  assert ( removeCell ( &m, 0, 1 ) );
  assert ( !removeCell ( &m, 0, 1 ) );
  assert ( !removeCell ( &m, 1, 2 ) );
  assert ( m . m_Rows
           && m . m_Rows -> m_Idx == 1
           && m . m_Rows -> m_Cells
           && m . m_Rows -> m_Cells -> m_Row == 1
           && m . m_Rows -> m_Cells -> m_Col == 0
           && m . m_Rows -> m_Cells -> m_Data == 20
           && m . m_Rows -> m_Cells -> m_Right
           && m . m_Rows -> m_Cells -> m_Right -> m_Row == 1
           && m . m_Rows -> m_Cells -> m_Right -> m_Col == 5
           && m . m_Rows -> m_Cells -> m_Right -> m_Data == 30
           && m . m_Rows -> m_Cells -> m_Right -> m_Right == nullptr );
  assert ( m . m_Rows -> m_Next
           && m . m_Rows -> m_Next -> m_Idx == 2
           && m . m_Rows -> m_Next -> m_Cells
           && m . m_Rows -> m_Next -> m_Cells -> m_Row == 2
           && m . m_Rows -> m_Next -> m_Cells -> m_Col == 1
           && m . m_Rows -> m_Next -> m_Cells -> m_Data == 40
           && m . m_Rows -> m_Next -> m_Cells -> m_Right == nullptr );
  assert ( m . m_Rows -> m_Next -> m_Next
           && m . m_Rows -> m_Next -> m_Next -> m_Idx == 230
           && m . m_Rows -> m_Next -> m_Next -> m_Cells
           && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Row == 230
           && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Col == 190
           && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Data == 50
           && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Right == nullptr );
  assert ( m . m_Rows -> m_Next -> m_Next -> m_Next == nullptr );
  assert ( m . m_Cols
           && m . m_Cols -> m_Idx == 0
           && m . m_Cols -> m_Cells
           && m . m_Cols -> m_Cells -> m_Row == 1
           && m . m_Cols -> m_Cells -> m_Col == 0
           && m . m_Cols -> m_Cells -> m_Data == 20
           && m . m_Cols -> m_Cells -> m_Down == nullptr );
  assert ( m . m_Cols -> m_Next
           && m . m_Cols -> m_Next -> m_Idx == 1
           && m . m_Cols -> m_Next -> m_Cells
           && m . m_Cols -> m_Next -> m_Cells -> m_Row == 2
           && m . m_Cols -> m_Next -> m_Cells -> m_Col == 1
           && m . m_Cols -> m_Next -> m_Cells -> m_Data == 40
           && m . m_Cols -> m_Next -> m_Cells -> m_Down == nullptr );
  assert ( m . m_Cols -> m_Next -> m_Next
           && m . m_Cols -> m_Next -> m_Next -> m_Idx == 5
           && m . m_Cols -> m_Next -> m_Next -> m_Cells
           && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Row == 1
           && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Col == 5
           && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Data == 30
           && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Down == nullptr );
  assert ( m . m_Cols -> m_Next -> m_Next -> m_Next
           && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Idx == 190
           && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells
           && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Row == 230
           && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Col == 190
           && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Data == 50
           && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Down == nullptr );
  assert ( m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Next == nullptr );
  assert ( m . m_Rows -> m_Cells == m . m_Cols -> m_Cells );
  assert ( m . m_Rows -> m_Cells -> m_Right == m . m_Cols -> m_Next -> m_Next -> m_Cells );
  assert ( m . m_Rows -> m_Next -> m_Cells == m . m_Cols -> m_Next -> m_Cells );
  assert ( m . m_Rows -> m_Next -> m_Next -> m_Cells == m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells );
  


  addSetCell ( &m, 1, 3, 60 );



  assert ( m . m_Rows
           && m . m_Rows -> m_Idx == 1
           && m . m_Rows -> m_Cells
           && m . m_Rows -> m_Cells -> m_Row == 1
           && m . m_Rows -> m_Cells -> m_Col == 0
           && m . m_Rows -> m_Cells -> m_Data == 20
           && m . m_Rows -> m_Cells -> m_Right
           && m . m_Rows -> m_Cells -> m_Right -> m_Row == 1
           && m . m_Rows -> m_Cells -> m_Right -> m_Col == 3
           && m . m_Rows -> m_Cells -> m_Right -> m_Data == 60
           && m . m_Rows -> m_Cells -> m_Right -> m_Right-> m_Row == 1
           && m . m_Rows -> m_Cells -> m_Right -> m_Right-> m_Data == 30
           && m . m_Rows -> m_Cells -> m_Right -> m_Right-> m_Col == 5
           && m . m_Rows -> m_Cells -> m_Right -> m_Right-> m_Right == nullptr);
  assert ( m . m_Rows -> m_Next
           && m . m_Rows -> m_Next -> m_Idx == 2
           && m . m_Rows -> m_Next -> m_Cells
           && m . m_Rows -> m_Next -> m_Cells -> m_Row == 2
           && m . m_Rows -> m_Next -> m_Cells -> m_Col == 1
           && m . m_Rows -> m_Next -> m_Cells -> m_Data == 40
           && m . m_Rows -> m_Next -> m_Cells -> m_Right == nullptr );
  assert ( m . m_Rows -> m_Next -> m_Next
           && m . m_Rows -> m_Next -> m_Next -> m_Idx == 230
           && m . m_Rows -> m_Next -> m_Next -> m_Cells
           && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Row == 230
           && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Col == 190
           && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Data == 50
           && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Right == nullptr );
  assert ( m . m_Rows -> m_Next -> m_Next -> m_Next == nullptr );
  assert ( m . m_Cols
           && m . m_Cols -> m_Idx == 0
           && m . m_Cols -> m_Cells
           && m . m_Cols -> m_Cells -> m_Row == 1
           && m . m_Cols -> m_Cells -> m_Col == 0
           && m . m_Cols -> m_Cells -> m_Data == 20
           && m . m_Cols -> m_Cells -> m_Down == nullptr );
  assert ( m . m_Cols -> m_Next
           && m . m_Cols -> m_Next -> m_Idx == 1
           && m . m_Cols -> m_Next -> m_Cells
           && m . m_Cols -> m_Next -> m_Cells -> m_Row == 2
           && m . m_Cols -> m_Next -> m_Cells -> m_Col == 1
           && m . m_Cols -> m_Next -> m_Cells -> m_Data == 40
           && m . m_Cols -> m_Next -> m_Cells -> m_Down == nullptr );
  assert ( m . m_Cols -> m_Next -> m_Next
           && m . m_Cols -> m_Next -> m_Next -> m_Idx == 3
           && m . m_Cols -> m_Next -> m_Next -> m_Cells
           && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Row == 1
           && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Col == 3
           && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Data == 60
           && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Down == nullptr);

  assert(  m . m_Cols -> m_Next -> m_Next -> m_Next 
           && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Idx == 5
           && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells
           && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Row == 1
           && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Col == 5
           && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Data == 30
           && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Down == nullptr );
  assert ( m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Next
           && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Next -> m_Idx == 190
           && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Next -> m_Cells
           && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Row == 230
           && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Col == 190
           && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Data == 50
           && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Down == nullptr );
  assert ( m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == nullptr );
  assert ( m . m_Rows -> m_Cells == m . m_Cols -> m_Cells );
  assert ( m . m_Rows -> m_Cells -> m_Right == m . m_Cols -> m_Next -> m_Next -> m_Cells );
  assert ( m . m_Rows -> m_Next -> m_Cells == m . m_Cols -> m_Next -> m_Cells );
  assert ( m . m_Rows -> m_Next -> m_Next -> m_Cells == m . m_Cols -> m_Next -> m_Next -> m_Next ->m_Next -> m_Cells );
  

  addSetCell ( &m, 1, 1, 80 );


  assert ( m . m_Rows
           && m . m_Rows -> m_Idx == 1
           && m . m_Rows -> m_Cells
           && m . m_Rows -> m_Cells -> m_Row == 1
           && m . m_Rows -> m_Cells -> m_Col == 0
           && m . m_Rows -> m_Cells -> m_Data == 20
           && m . m_Rows -> m_Cells -> m_Right -> m_Row == 1
           && m . m_Rows -> m_Cells -> m_Right -> m_Col == 1
           && m . m_Rows -> m_Cells -> m_Right -> m_Data == 80
           && m . m_Rows -> m_Cells -> m_Right -> m_Right
           && m . m_Rows -> m_Cells -> m_Right -> m_Right -> m_Row == 1
           && m . m_Rows -> m_Cells -> m_Right -> m_Right -> m_Col == 3
           && m . m_Rows -> m_Cells -> m_Right -> m_Right -> m_Data == 60
           && m . m_Rows -> m_Cells -> m_Right -> m_Right -> m_Right-> m_Row == 1
           && m . m_Rows -> m_Cells -> m_Right -> m_Right -> m_Right-> m_Data == 30
           && m . m_Rows -> m_Cells -> m_Right -> m_Right -> m_Right-> m_Col == 5
           && m . m_Rows -> m_Cells -> m_Right -> m_Right -> m_Right-> m_Right == nullptr);
  assert ( m . m_Rows -> m_Next
           && m . m_Rows -> m_Next -> m_Idx == 2
           && m . m_Rows -> m_Next -> m_Cells
           && m . m_Rows -> m_Next -> m_Cells -> m_Row == 2
           && m . m_Rows -> m_Next -> m_Cells -> m_Col == 1
           && m . m_Rows -> m_Next -> m_Cells -> m_Data == 40
           && m . m_Rows -> m_Next -> m_Cells -> m_Right == nullptr );
  assert ( m . m_Rows -> m_Next -> m_Next
           && m . m_Rows -> m_Next -> m_Next -> m_Idx == 230
           && m . m_Rows -> m_Next -> m_Next -> m_Cells
           && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Row == 230
           && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Col == 190
           && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Data == 50
           && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Right == nullptr );
  assert ( m . m_Rows -> m_Next -> m_Next -> m_Next == nullptr );



  assert ( m . m_Cols
           && m . m_Cols -> m_Idx == 0
           && m . m_Cols -> m_Cells
           && m . m_Cols -> m_Cells -> m_Row == 1
           && m . m_Cols -> m_Cells -> m_Col == 0
           && m . m_Cols -> m_Cells -> m_Data == 20
           && m . m_Cols -> m_Cells -> m_Down == nullptr );

  assert  ( m . m_Cols -> m_Next
           && m . m_Cols -> m_Next -> m_Idx == 1
           && m . m_Cols -> m_Next -> m_Cells
           && m . m_Cols -> m_Next -> m_Cells -> m_Row == 1
           && m . m_Cols -> m_Next -> m_Cells -> m_Col == 1
           && m . m_Cols -> m_Next -> m_Cells -> m_Data == 80
           && m . m_Cols -> m_Next -> m_Cells -> m_Down 
           && m . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Row == 2
           && m . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Col == 1
           && m . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Data == 40
           && m . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Down == nullptr);

  assert ( m . m_Cols -> m_Next -> m_Next
           && m . m_Cols -> m_Next -> m_Next -> m_Idx == 3
           && m . m_Cols -> m_Next -> m_Next -> m_Cells
           && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Row == 1
           && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Col == 3
           && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Data == 60
           && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Down == nullptr);

  assert(  m . m_Cols -> m_Next -> m_Next -> m_Next 
           && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Idx == 5
           && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells
           && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Row == 1
           && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Col == 5
           && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Data == 30
           && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Down == nullptr );
  assert ( m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Next
           && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Next -> m_Idx == 190
           && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Next -> m_Cells
           && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Row == 230
           && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Col == 190
           && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Data == 50
           && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Down == nullptr );
  assert ( m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == nullptr );
  assert ( m . m_Rows -> m_Cells == m . m_Cols -> m_Cells );
  assert ( m . m_Rows -> m_Cells -> m_Right == m . m_Cols -> m_Next -> m_Cells );
  assert ( m . m_Rows -> m_Next -> m_Cells == m . m_Cols -> m_Next -> m_Cells -> m_Down);
  assert ( m . m_Rows -> m_Next -> m_Next -> m_Cells == m . m_Cols -> m_Next -> m_Next -> m_Next ->m_Next -> m_Cells );
  
  
  freeMatrix ( &m );

  TSPARSEMATRIX  m2;
  initMatrix ( &m2 );
  addSetCell ( &m2, 0, 1, 10 );
  addSetCell ( &m2, 1, 0, 20 );
  addSetCell ( &m2, 0, 0, 0 );
  addSetCell ( &m2, 3, 1, 60 );
  addSetCell ( &m2, 0, 3, 40 );
  addSetCell ( &m2, 0, 2, 100 );
  addSetCell ( &m2, 1, 1, 200 );//up cell not pointing to the curr->down 



  assert ( m2 . m_Rows
           && m2 . m_Rows -> m_Idx == 0
           && m2 . m_Rows -> m_Cells
           && m2 . m_Rows -> m_Cells -> m_Row == 0
           && m2 . m_Rows -> m_Cells -> m_Col == 0
           && m2 . m_Rows -> m_Cells -> m_Data == 0
           && m2 . m_Rows -> m_Cells -> m_Right -> m_Row == 0
           && m2 . m_Rows -> m_Cells -> m_Right -> m_Col == 1
           && m2 . m_Rows -> m_Cells -> m_Right -> m_Data == 10
           && m2 . m_Rows -> m_Cells -> m_Right -> m_Right
           && m2 . m_Rows -> m_Cells -> m_Right -> m_Right -> m_Row == 0
           && m2 . m_Rows -> m_Cells -> m_Right -> m_Right -> m_Col == 2
           && m2 . m_Rows -> m_Cells -> m_Right -> m_Right -> m_Data == 100
           && m2 . m_Rows -> m_Cells -> m_Right -> m_Right -> m_Right -> m_Row == 0
           && m2 . m_Rows -> m_Cells -> m_Right -> m_Right -> m_Right -> m_Col == 3
           && m2 . m_Rows -> m_Cells -> m_Right -> m_Right -> m_Right -> m_Data == 40
           && m2 . m_Rows -> m_Cells -> m_Right -> m_Right -> m_Right -> m_Right == nullptr);
           

  assert ( m2 . m_Rows -> m_Next
           && m2 . m_Rows -> m_Next -> m_Idx == 1
           && m2 . m_Rows -> m_Next -> m_Cells
           && m2 . m_Rows -> m_Next -> m_Cells -> m_Row == 1
           && m2 . m_Rows -> m_Next -> m_Cells -> m_Col == 0
           && m2 . m_Rows -> m_Next -> m_Cells -> m_Data == 20
           && m2 . m_Rows -> m_Next -> m_Cells -> m_Right
           && m2 . m_Rows -> m_Next -> m_Cells -> m_Right -> m_Row == 1
           && m2 . m_Rows -> m_Next -> m_Cells -> m_Right -> m_Col == 1
           && m2 . m_Rows -> m_Next -> m_Cells -> m_Right -> m_Data == 200
           && m2 . m_Rows -> m_Next -> m_Cells -> m_Right -> m_Right == nullptr);
           


  assert ( m2 . m_Rows -> m_Next -> m_Next -> m_Idx == 3);
  assert ( m2 . m_Rows -> m_Next -> m_Next -> m_Cells);
  assert ( m2 . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Row == 3);
  assert ( m2 . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Col == 1);
  assert ( m2 . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Data == 60);
  assert ( m2 . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Right == nullptr );


  assert ( m2.m_Rows -> m_Next -> m_Next -> m_Next== nullptr);
//     printMatrix(&m2);
//   printMatrix2(&m2);
//     TROWCOL * tmp;
//   tmp = m2.m_Rows;
//   while ( tmp != nullptr ){
//     TCELL * tmpCell = tmp->m_Cells;  
//     while ( tmpCell  != nullptr ){
//       printf("%d\n" , tmpCell->m_Data);
//       tmpCell = tmpCell->m_Right;
//     }
//     tmp = tmp->m_Next;
//   }

  assert ( m2 . m_Cols
           && m2 . m_Cols -> m_Idx == 0
           && m2 . m_Cols -> m_Cells
           && m2 . m_Cols -> m_Cells -> m_Row == 0
           && m2 . m_Cols -> m_Cells -> m_Col == 0
           && m2 . m_Cols -> m_Cells -> m_Data == 00
           && m2 . m_Cols -> m_Cells -> m_Down
           && m2 . m_Cols -> m_Cells -> m_Down -> m_Row == 1
           && m2 . m_Cols -> m_Cells -> m_Down -> m_Col == 0
           && m2 . m_Cols -> m_Cells -> m_Down -> m_Data == 20
           && m2 . m_Cols -> m_Cells -> m_Down -> m_Down == nullptr);

  assert  ( m2 . m_Cols -> m_Next
           && m2 . m_Cols -> m_Next -> m_Idx == 1
           && m2 . m_Cols -> m_Next -> m_Cells
           && m2 . m_Cols -> m_Next -> m_Cells -> m_Row == 0
           && m2 . m_Cols -> m_Next -> m_Cells -> m_Col == 1
           && m2 . m_Cols -> m_Next -> m_Cells -> m_Data == 10
           && m2 . m_Cols -> m_Next -> m_Cells -> m_Down
           && m2 . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Row == 1
           && m2 . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Col == 1
           && m2 . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Data == 200
           && m2 . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Down 
           && m2 . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Down -> m_Row == 3
           && m2 . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Down -> m_Col == 1
           && m2 . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Down -> m_Down == nullptr
           && m2 . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Down -> m_Data == 60);

  assert  ( m2 . m_Cols -> m_Next -> m_Next
           && m2 . m_Cols -> m_Next -> m_Next -> m_Idx == 2
           && m2 . m_Cols -> m_Next -> m_Next -> m_Cells
           && m2 . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Row == 0
           && m2 . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Col == 2
           && m2 . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Down == nullptr
           && m2 . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Data == 100);         



  assert  ( m2 . m_Cols -> m_Next -> m_Next -> m_Next
           && m2 . m_Cols -> m_Next -> m_Next -> m_Next -> m_Idx == 3
           && m2 . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells
           && m2 . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Row == 0
           && m2 . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Col == 3
           && m2 . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Down == nullptr
           && m2 . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Data == 40);         

  assert  ( m2 . m_Cols -> m_Next -> m_Next -> m_Next -> m_Next == nullptr);


  assert ( removeCell ( &m2, 0, 2 ) );
  assert ( removeCell ( &m2, 1, 1 ) );



  assert ( m2 . m_Rows
           && m2 . m_Rows -> m_Idx == 0
           && m2 . m_Rows -> m_Cells
           && m2 . m_Rows -> m_Cells -> m_Row == 0
           && m2 . m_Rows -> m_Cells -> m_Col == 0
           && m2 . m_Rows -> m_Cells -> m_Data == 0
           && m2 . m_Rows -> m_Cells -> m_Right -> m_Row == 0
           && m2 . m_Rows -> m_Cells -> m_Right -> m_Col == 1
           && m2 . m_Rows -> m_Cells -> m_Right -> m_Data == 10
           && m2 . m_Rows -> m_Cells -> m_Right -> m_Right -> m_Row == 0
           && m2 . m_Rows -> m_Cells -> m_Right -> m_Right -> m_Col == 3
           && m2 . m_Rows -> m_Cells -> m_Right -> m_Right -> m_Data == 40
           && m2 . m_Rows -> m_Cells -> m_Right -> m_Right -> m_Right == nullptr);
           

  assert ( m2 . m_Rows -> m_Next
           && m2 . m_Rows -> m_Next -> m_Idx == 1
           && m2 . m_Rows -> m_Next -> m_Cells
           && m2 . m_Rows -> m_Next -> m_Cells -> m_Row == 1
           && m2 . m_Rows -> m_Next -> m_Cells -> m_Col == 0
           && m2 . m_Rows -> m_Next -> m_Cells -> m_Data == 20
           && m2 . m_Rows -> m_Next -> m_Cells -> m_Right == nullptr);
           


  assert ( m2 . m_Rows -> m_Next -> m_Next -> m_Idx == 3);
  assert ( m2 . m_Rows -> m_Next -> m_Next -> m_Cells);
  assert ( m2 . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Row == 3);
  assert ( m2 . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Col == 1);
  assert ( m2 . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Data == 60);
  assert ( m2 . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Right == nullptr );


  assert ( m2.m_Rows -> m_Next -> m_Next -> m_Next== nullptr);

  assert ( m2 . m_Cols
           && m2 . m_Cols -> m_Idx == 0
           && m2 . m_Cols -> m_Cells
           && m2 . m_Cols -> m_Cells -> m_Row == 0
           && m2 . m_Cols -> m_Cells -> m_Col == 0
           && m2 . m_Cols -> m_Cells -> m_Data == 00
           && m2 . m_Cols -> m_Cells -> m_Down
           && m2 . m_Cols -> m_Cells -> m_Down -> m_Row == 1
           && m2 . m_Cols -> m_Cells -> m_Down -> m_Col == 0
           && m2 . m_Cols -> m_Cells -> m_Down -> m_Data == 20
           && m2 . m_Cols -> m_Cells -> m_Down -> m_Down == nullptr);

  assert  ( m2 . m_Cols -> m_Next
           && m2 . m_Cols -> m_Next -> m_Idx == 1
           && m2 . m_Cols -> m_Next -> m_Cells
           && m2 . m_Cols -> m_Next -> m_Cells -> m_Row == 0
           && m2 . m_Cols -> m_Next -> m_Cells -> m_Col == 1
           && m2 . m_Cols -> m_Next -> m_Cells -> m_Data == 10
           && m2 . m_Cols -> m_Next -> m_Cells -> m_Down
           && m2 . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Row == 3
           && m2 . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Col == 1
           && m2 . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Down == nullptr
           && m2 . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Data == 60);



  assert  ( m2 . m_Cols -> m_Next -> m_Next
           && m2 . m_Cols -> m_Next -> m_Next -> m_Idx == 3
           && m2 . m_Cols -> m_Next -> m_Next -> m_Cells
           && m2 . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Row == 0
           && m2 . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Col == 3
           && m2 . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Down == nullptr
           && m2 . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Data == 40);         
  assert  ( m2 . m_Cols -> m_Next -> m_Next -> m_Next == nullptr);


  addSetCell ( &m2, 0, 0, 1000 );
  addSetCell ( &m2, 0, 1, 1010 );


  assert ( m2 . m_Rows
           && m2 . m_Rows -> m_Idx == 0
           && m2 . m_Rows -> m_Cells
           && m2 . m_Rows -> m_Cells -> m_Row == 0
           && m2 . m_Rows -> m_Cells -> m_Col == 0
           && m2 . m_Rows -> m_Cells -> m_Data == 1000
           && m2 . m_Rows -> m_Cells -> m_Right -> m_Row == 0
           && m2 . m_Rows -> m_Cells -> m_Right -> m_Col == 1
           && m2 . m_Rows -> m_Cells -> m_Right -> m_Data == 1010
           && m2 . m_Rows -> m_Cells -> m_Right -> m_Right -> m_Row == 0
           && m2 . m_Rows -> m_Cells -> m_Right -> m_Right -> m_Col == 3
           && m2 . m_Rows -> m_Cells -> m_Right -> m_Right -> m_Data == 40
           && m2 . m_Rows -> m_Cells -> m_Right -> m_Right -> m_Right == nullptr);
           

  assert ( m2 . m_Rows -> m_Next
           && m2 . m_Rows -> m_Next -> m_Idx == 1
           && m2 . m_Rows -> m_Next -> m_Cells
           && m2 . m_Rows -> m_Next -> m_Cells -> m_Row == 1
           && m2 . m_Rows -> m_Next -> m_Cells -> m_Col == 0
           && m2 . m_Rows -> m_Next -> m_Cells -> m_Data == 20
           && m2 . m_Rows -> m_Next -> m_Cells -> m_Right == nullptr);
           


  assert ( m2 . m_Rows -> m_Next -> m_Next -> m_Idx == 3);
  assert ( m2 . m_Rows -> m_Next -> m_Next -> m_Cells);
  assert ( m2 . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Row == 3);
  assert ( m2 . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Col == 1);
  assert ( m2 . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Data == 60);
  assert ( m2 . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Right == nullptr );


  assert ( m2.m_Rows -> m_Next -> m_Next -> m_Next== nullptr);

  assert ( m2 . m_Cols
           && m2 . m_Cols -> m_Idx == 0
           && m2 . m_Cols -> m_Cells
           && m2 . m_Cols -> m_Cells -> m_Row == 0
           && m2 . m_Cols -> m_Cells -> m_Col == 0
           && m2 . m_Cols -> m_Cells -> m_Data == 1000
           && m2 . m_Cols -> m_Cells -> m_Down
           && m2 . m_Cols -> m_Cells -> m_Down -> m_Row == 1
           && m2 . m_Cols -> m_Cells -> m_Down -> m_Col == 0
           && m2 . m_Cols -> m_Cells -> m_Down -> m_Data == 20
           && m2 . m_Cols -> m_Cells -> m_Down -> m_Down == nullptr);

  assert  ( m2 . m_Cols -> m_Next
           && m2 . m_Cols -> m_Next -> m_Idx == 1
           && m2 . m_Cols -> m_Next -> m_Cells
           && m2 . m_Cols -> m_Next -> m_Cells -> m_Row == 0
           && m2 . m_Cols -> m_Next -> m_Cells -> m_Col == 1
           && m2 . m_Cols -> m_Next -> m_Cells -> m_Data == 1010
           && m2 . m_Cols -> m_Next -> m_Cells -> m_Down
           && m2 . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Row == 3
           && m2 . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Col == 1
           && m2 . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Down == nullptr
           && m2 . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Data == 60);



  assert  ( m2 . m_Cols -> m_Next -> m_Next
           && m2 . m_Cols -> m_Next -> m_Next -> m_Idx == 3
           && m2 . m_Cols -> m_Next -> m_Next -> m_Cells
           && m2 . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Row == 0
           && m2 . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Col == 3
           && m2 . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Down == nullptr
           && m2 . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Data == 40);         
  assert  ( m2 . m_Cols -> m_Next -> m_Next -> m_Next == nullptr);


  assert ( removeCell ( &m2, 0, 3 ) );

  assert ( !removeCell ( &m2, 3 , 0  ) );

  assert ( removeCell ( &m2, 3 , 1  ) );



  assert ( m2 . m_Rows
           && m2 . m_Rows -> m_Idx == 0
           && m2 . m_Rows -> m_Cells
           && m2 . m_Rows -> m_Cells -> m_Row == 0
           && m2 . m_Rows -> m_Cells -> m_Col == 0
           && m2 . m_Rows -> m_Cells -> m_Data == 1000
           && m2 . m_Rows -> m_Cells -> m_Right -> m_Row == 0
           && m2 . m_Rows -> m_Cells -> m_Right -> m_Col == 1
           && m2 . m_Rows -> m_Cells -> m_Right -> m_Data == 1010
           && m2 . m_Rows -> m_Cells -> m_Right -> m_Right  == nullptr);
           

  assert ( m2 . m_Rows -> m_Next
           && m2 . m_Rows -> m_Next -> m_Idx == 1
           && m2 . m_Rows -> m_Next -> m_Cells
           && m2 . m_Rows -> m_Next -> m_Cells -> m_Row == 1
           && m2 . m_Rows -> m_Next -> m_Cells -> m_Col == 0
           && m2 . m_Rows -> m_Next -> m_Cells -> m_Data == 20
           && m2 . m_Rows -> m_Next -> m_Cells -> m_Right == nullptr);
           
  
  assert ( m2.m_Rows -> m_Next -> m_Next == nullptr);

  assert ( m2 . m_Cols
           && m2 . m_Cols -> m_Idx == 0
           && m2 . m_Cols -> m_Cells
           && m2 . m_Cols -> m_Cells -> m_Row == 0
           && m2 . m_Cols -> m_Cells -> m_Col == 0
           && m2 . m_Cols -> m_Cells -> m_Data == 1000
           && m2 . m_Cols -> m_Cells -> m_Down
           && m2 . m_Cols -> m_Cells -> m_Down -> m_Row == 1
           && m2 . m_Cols -> m_Cells -> m_Down -> m_Col == 0
           && m2 . m_Cols -> m_Cells -> m_Down -> m_Data == 20
           && m2 . m_Cols -> m_Cells -> m_Down -> m_Down == nullptr);


  assert  ( m2 . m_Cols -> m_Next
           && m2 . m_Cols -> m_Next -> m_Idx == 1
           && m2 . m_Cols -> m_Next -> m_Cells
           && m2 . m_Cols -> m_Next -> m_Cells -> m_Row == 0
           && m2 . m_Cols -> m_Next -> m_Cells -> m_Col == 1
           && m2 . m_Cols -> m_Next -> m_Cells -> m_Data == 1010
           && m2 . m_Cols -> m_Next -> m_Cells -> m_Down == nullptr);
           
     
  assert  ( m2 . m_Cols -> m_Next -> m_Next == nullptr);

  freeMatrix ( &m2);


  TSPARSEMATRIX m3;
  initMatrix( &m3);
  
  addSetCell(&m3, 5,5, 10);
  assert ( ! removeCell( &m3, 5,8));
  assert ( removeCell ( &m3, 5 , 5));
  freeMatrix(&m3);


  TSPARSEMATRIX m5;
  initMatrix(&m5);
  addSetCell(&m5 , 2,2 , 20);
  addSetCell(&m5 , 1,2 , 10);
  freeMatrix(&m5);

  TSPARSEMATRIX m4;
  initMatrix( &m4);
  addSetCell(&m4 , 0,2 , 10); //
  addSetCell(&m4 , 0,1 , 22); //
  addSetCell(&m4 , 2,3 , 23); //
  addSetCell(&m4 , 0,1 , 40); //
  addSetCell(&m4 , 0,3 , 24); //
  addSetCell(&m4 , 2,4 , 25); //
  addSetCell(&m4 , 5,5 , 26); //
  addSetCell(&m4 , 4,4 , 27); //
  addSetCell(&m4 , 2,2 , 200);
  addSetCell(&m4 , 1,4 , 300);
  addSetCell(&m4 , 3,4 , 400);


  assert ( ! removeCell (&m4 , 0 , 0 ));
  assert ( ! removeCell (&m4 , 1 , 9 ));
  assert ( ! removeCell (&m4 , 8 , 3  ));
  assert ( ! removeCell (&m4 , 10 , 10 ));


  assert (  removeCell (&m4 , 2 , 3 ));
  assert (  removeCell (&m4 , 0 , 1 ));
  assert (  removeCell (&m4 , 0 , 2 ));
  assert (  removeCell (&m4 , 2 , 4 ));
  assert (  removeCell (&m4 , 5 , 5 ));
  assert (  removeCell (&m4 , 4 , 4 ));
  assert (  removeCell (&m4 , 0 , 3 ));
 

  freeMatrix(&m4);
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
