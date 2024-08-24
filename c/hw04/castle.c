#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <math.h>
constexpr int MAP_MAX = 200;
#endif /* __PROGTEST__ */

typedef struct
{
  int start;
  int end;
  int height;
} Interval;

int max(int a, int b)
{
  return (a >= b) ? a : b;
}

int min(int a, int b)
{
  return (a <= b) ? a : b;
}

Interval intersection(Interval a, Interval b)
{  
  Interval res;
  int start = max(a.start, b.start);
  int end = min(a.end, b.end);
  if(start > end)
  {
    res.start  = 0;
    res.end = 0;
    res.height = 0;
  }
  else
  {
    res.start = start;
    res.end = end;
    res.height = b.height - a.height + 1;
  }

  return res;  
}

int computeArea(Interval a, Interval b)
{
  Interval recinfo = intersection(a, b);
  int width = recinfo.end - recinfo.start + 1;
  int height = recinfo.height; // Use the minimum height
  int area;
  //checking overflow;
  if(width > INT_MAX / height)
  {
    area = 0; 
  }
  else
  {
    area = width * height;
  }
  return area;
}


void checkSides(const int arr[][MAP_MAX], int row, int colum, 
          Interval *interval, int * const max_right, int * const max_left, int count, int castle)
{ 
  interval[count].height = row;
  int c = colum;
  while( c - 1 >= *max_left && castle > arr[row][c - 1])
  {
    // printf("checking left\n");
    c--;
  }
  *max_left = c;
  interval[count].start = c;

  while( colum + 1 <= *max_right &&  castle > arr[row][colum + 1])
  {
    // printf("checking right\n");
    colum++;
  }
  *max_right = colum;
  interval[count].end = colum;

}

int checksUpper(const int arr[][MAP_MAX], int row, int colum, int castle)
{

  if(arr[row - 1][colum] < castle)
  {
    return 1;
  }
  else
    return 0;
}

int checksDown(const int arr[][MAP_MAX], int row, int colum, int castle)
{
  if(arr[row + 1][colum] < castle)
  {
    return 1;
  }
  else
    return 0;
}

/**
*@return biggest area of rectangle
*@arg index of castle, altitude array
*/
int biggestArea(const int altitude[][MAP_MAX], int row, int colum, const int size)
{
  int castle = altitude[row][colum];
  Interval interval1[MAP_MAX];
  Interval interval2[MAP_MAX];
  int upperNum = 0, downNum = 0;
  int count = 0;
  int r = row;
  int max_right = size - 1, max_left = 0;
  //instead of size pass the index 
  checkSides(altitude, r, colum, interval1, &max_right, &max_left, count, castle);
  count++;
  while (r - 1 >= 0 && checksUpper(altitude, r, colum, castle) == 1)
  {
    checkSides(altitude, r - 1, colum, interval1, &max_right, &max_left, count, castle);
    r--;
    upperNum++; //same as interval1.used
    count++;
  }

  count = 0;
  max_right = size -1, max_left = 0;
  checkSides(altitude, row, colum, interval2, &max_right, &max_left, count, castle);
  count++;
  while ( (row + 1) < size && checksDown(altitude, row, colum, castle) == 1)
  {
    checkSides(altitude, row + 1, colum, interval2, &max_right, &max_left, count, castle);
    row++;
    downNum++;
    count++;
  }

  // concatenating all
  int maxArea = 0;
  for (int i = 0; i <= upperNum; i++)
  {
    for (int j = 0; j <= downNum; j++)
    {
      int area = computeArea(interval1[i], interval2[j]);
      maxArea = (area >= maxArea) ? area : maxArea;
    }
  }

  return maxArea;
}

void castleArea ( int altitude[][MAP_MAX], int size, int area[][MAP_MAX] )
{
  for(int i = 0; i < size; i++)
  {
    for(int j = 0; j < size; j++)
    {
      // printf("arr[%d][%d]:\n", i, j);
      int area_res = biggestArea(altitude, i, j, size);
      area[i][j] = area_res;
      
    }
  }

}

#ifndef __PROGTEST__
bool identicalMap ( const int a[][MAP_MAX], const int b[][MAP_MAX], int n )
{
  if(n > MAP_MAX)
  {
    return false;
  }
  for(int i = 0; i < n; i++)
  {
    for(int j = 0; j < n; j++)
    {
      if(a[i][j] == 0)
      {
        return false;
      }
      // printf("a,b[%d][%d] = a: %d b: %d\n",i,j,a[i][j], b[i][j]);
      if(a[i][j] != b[i][j])
      {
        return false;
      }
    }
  }
  return true;
}
int main ( int argc, char * argv [] )
{
  static int result[MAP_MAX][MAP_MAX];

  static int alt0[MAP_MAX][MAP_MAX] =
  {
    { 1, 2 },
    { 3, 4 }
  };
  static int area0[MAP_MAX][MAP_MAX] =
  {
    { 1, 2 },
    { 2, 4 }
  };
  castleArea ( alt0, 2, result );
  assert ( identicalMap ( result, area0, 2 ) );
  castleArea ( alt0, 2, result );
  identicalMap ( result, area0, 2 );
  static int alt1[MAP_MAX][MAP_MAX] =
  {
    { 2, 7, 1, 9 },
    { 3, 5, 0, 2 },
    { 1, 6, 3, 5 },
    { 1, 2, 2, 8 }
  };
  static int area1[MAP_MAX][MAP_MAX] =
  {
    { 1, 12, 2, 16 },
    { 4, 4, 1, 2 },
    { 1, 9, 4, 4 },
    { 1, 2, 1, 12 }
  };
  castleArea ( alt1, 4, result );
  assert ( identicalMap ( result, area1, 4 ) );
  static int alt2[MAP_MAX][MAP_MAX] =
  {
    { 1, 2, 3, 4 },
    { 2, 3, 4, 5 },
    { 3, 4, 5, 6 },
    { 4, 5, 6, 7 }
  };
  static int area2[MAP_MAX][MAP_MAX] =
  {
    { 1, 2, 3, 4 },
    { 2, 4, 6, 8 },
    { 3, 6, 9, 12 },
    { 4, 8, 12, 16 }
  };
  castleArea ( alt2, 4, result );
  assert ( identicalMap ( result, area2, 4 ) );
  static int alt3[MAP_MAX][MAP_MAX] =
  {
    { 7, 6, 5, 4 },
    { 6, 5, 4, 5 },
    { 5, 4, 5, 6 },
    { 4, 5, 6, 7 }
  };
  static int area3[MAP_MAX][MAP_MAX] =
  {
    { 12, 6, 2, 1 },
    { 6, 2, 1, 2 },
    { 2, 1, 2, 6 },
    { 1, 2, 6, 12 }
  };
  castleArea ( alt3, 4, result );
  assert ( identicalMap ( result, area3, 4 ) );
  static int alt4[MAP_MAX][MAP_MAX] =
  {
    { 1, 1, 1, 1, 1 },
    { 1, 1, 1, 1, 1 },
    { 1, 1, 2, 1, 1 },
    { 1, 1, 1, 1, 1 },
    { 1, 1, 1, 1, 1 }
  };
  static int area4[MAP_MAX][MAP_MAX] =
  {
    { 1, 1, 1, 1, 1 },
    { 1, 1, 1, 1, 1 },
    { 1, 1, 25, 1, 1 },
    { 1, 1, 1, 1, 1 },
    { 1, 1, 1, 1, 1 }
  };
  castleArea ( alt4, 5, result );
  assert ( identicalMap ( result, area4, 5 ) );



static int alt6[MAP_MAX][MAP_MAX] =
  {
    { 1, 1, 1, 1, 1 },
    { -2, -1, -2, -2, -2 },
    { 1, 1, 1, 1, 1 },
    { 1, 1, 1, 1, 1 },
    { 1, 1, 1, 1, 1 }
  };
  static int area6[MAP_MAX][MAP_MAX] =
  {
    { 2, 2, 2, 2, 2 },
    { 1, 5, 1, 1, 1 },
    { 2, 2, 2, 2, 2 },
    { 1, 1, 1, 1, 1 },
    { 1, 1, 1, 1, 1 }
  };
  castleArea ( alt6, 5, result );
  assert ( identicalMap ( result, area6, 5 ) );

  static int alt7[MAP_MAX][MAP_MAX] =
  {
  { 913, 633, 607, 179, 80, 971, 479, 374, 48, 705, 289, 60, 657, 652, 562, 113, 152, 970, 361, 884, 161, 96, 33, 382, 6, 681, 206, 613, 241 },
  { 967, 505, 506, 601, 112, 38, 33, 435, 869, 408, 836, 574, 49, 896, 231, 701, 810, 697, 853, 133, 58, 89, 646, 154, 123, 380, 513, 156, 586 },
  { 478, 397, 554, 335, 256, 507, 447, 294, 540, 235, 163, 300, 71, 738, 349, 319, 321, 50, 129, 18, 256, 614, 429, 697, 261, 935, 172, 641, 448 },
  { 328, 580, 278, 78, 134, 613, 334, 993, 413, 628, 885, 648, 143, 186, 71, 233, 535, 390, 555, 938, 871, 925, 546, 486, 354, 243, 99, 642, 416 },
  { 740, 90, 744, 320, 721, 174, 806, 334, 508, 151, 99, 488, 37, 99, 632, 223, 170, 217, 110, 912, 772, 48, 784, 50, 946, 622, 756, 190, 721 },
  { 398, 606, 461, 841, 702, 134, 562, 877, 292, 248, 737, 444, 348, 226, 481, 447, 210, 56, 970, 427, 166, 882, 552, 567, 18, 954, 513, 640, 710 },
  { 55, 361, 461, 661, 175, 302, 716, 661, 216, 945, 953, 464, 682, 749, 164, 260, 582, 964, 822, 638, 934, 250, 157, 168, 154, 724, 187, 108, 589 },
  { 827, 170, 645, 541, 631, 658, 68, 285, 726, 729, 501, 671, 34, 318, 706, 784, 482, 966, 718, 446, 789, 709, 732, 391, 866, 901, 897, 942, 88 },
  { 357, 531, 267, 879, 528, 160, 863, 187, 228, 148, 913, 309, 2, 937, 344, 320, 643, 480, 802, 961, 550, 601, 102, 259, 333, 845, 125, 234, 742 },
  { 419, 674, 451, 303, 942, 331, 831, 454, 546, 370, 683, 46, 284, 344, 48, 221, 40, 720, 216, 872, 523, 177, 423, 476, 280, 682, 809, 125, 160 },
  { 396, 220, 579, 422, 671, 882, 716, 354, 66, 171, 900, 436, 206, 947, 72, 550, 347, 293, 591, 68, 861, 463, 943, 39, 886, 419, 319, 921, 580 },
  { 796, 81, 328, 16, 12, 751, 40, 895, 467, 394, 961, 638, 647, 749, 844, 946, 822, 747, 293, 115, 338, 361, 329, 153, 304, 368, 392, 75, 39 },
  { 313, 656, 835, 394, 984, 204, 406, 735, 244, 653, 555, 990, 966, 193, 989, 716, 390, 935, 538, 137, 229, 5, 827, 942, 334, 980, 247, 702, 372 },
  { 322, 93, 37, 330, 281, 431, 667, 485, 190, 754, 81, 195, 309, 423, 162, 855, 413, 878, 245, 700, 768, 734, 929, 773, 561, 872, 460, 893, 471 },
  { 514, 618, 145, 608, 655, 828, 889, 439, 495, 726, 981, 249, 159, 176, 911, 934, 690, 766, 699, 568, 363, 400, 688, 97, 681, 814, 10, 905, 274 },
  { 255, 728, 788, 873, 874, 748, 881, 54, 989, 672, 549, 715, 653, 150, 226, 181, 61, 161, 872, 179, 860, 792, 542, 612, 481, 639, 294, 295, 1 },
  { 551, 921, 257, 280, 709, 482, 506, 458, 715, 560, 799, 387, 461, 515, 392, 611, 93, 926, 25, 606, 798, 204, 819, 942, 747, 431, 423, 738, 77 },
  { 70, 740, 981, 991, 349, 261, 53, 831, 119, 863, 899, 679, 662, 286, 140, 529, 31, 103, 975, 957, 128, 581, 107, 333, 400, 49, 432, 184, 825 },
  { 170, 261, 895, 262, 242, 239, 611, 855, 292, 795, 974, 507, 694, 653, 521, 332, 145, 51, 715, 249, 26, 24, 729, 607, 131, 414, 360, 181, 846 },
  { 544, 6, 369, 157, 253, 631, 400, 492, 595, 607, 136, 390, 582, 995, 436, 587, 517, 120, 85, 568, 836, 686, 946, 860, 415, 905, 344, 182, 265 },
  { 525, 380, 161, 883, 749, 319, 136, 733, 71, 981, 328, 678, 469, 70, 612, 465, 858, 552, 982, 330, 637, 902, 166, 323, 200, 379, 90, 105, 723 },
  { 624, 723, 600, 5, 884, 483, 106, 203, 971, 839, 626, 304, 519, 305, 774, 941, 269, 591, 151, 821, 925, 482, 458, 179, 0, 133, 379, 379, 576 },
  { 836, 454, 200, 559, 406, 205, 796, 241, 312, 351, 213, 503, 978, 869, 375, 635, 995, 316, 904, 586, 468, 726, 863, 302, 536, 42, 302, 670, 421 },
  { 34, 598, 610, 488, 798, 521, 895, 356, 669, 136, 20, 21, 701, 875, 351, 923, 250, 986, 918, 567, 242, 857, 387, 968, 720, 689, 505, 763, 991 },
  { 527, 536, 25, 125, 146, 866, 275, 20, 113, 983, 689, 601, 355, 710, 303, 231, 413, 226, 481, 399, 496, 400, 642, 353, 139, 962, 74, 180, 819 },
  { 189, 172, 346, 725, 549, 823, 224, 767, 451, 244, 880, 434, 933, 482, 790, 996, 137, 373, 409, 363, 854, 161, 211, 607, 155, 565, 98, 117, 639 },
  { 279, 937, 828, 451, 635, 905, 352, 459, 129, 120, 910, 725, 352, 696, 11, 834, 838, 359, 971, 211, 120, 686, 418, 281, 898, 25, 788, 463, 123 },
  { 906, 454, 402, 195, 634, 205, 830, 539, 910, 641, 21, 382, 903, 98, 734, 600, 461, 569, 438, 820, 892, 2, 941, 579, 420, 574, 477, 445, 363 },
  { 292, 920, 621, 746, 675, 168, 380, 232, 350, 271, 142, 992, 644, 524, 247, 743, 259, 199, 204, 180, 990, 377, 72, 992, 670, 651, 412, 244, 480 }
};

  static int area7[MAP_MAX][MAP_MAX]= 
  {
    { 5, 16, 12, 2, 1, 87, 6, 2, 1, 11, 2, 1, 12, 4, 4, 1, 2, 120, 1, 22, 4, 2, 1, 5, 1, 18, 1, 6, 1 },
  { 77, 2, 2, 14, 3, 2, 1, 4, 24, 2, 12, 4, 1, 54, 1, 14, 33, 5, 22, 3, 1, 2, 10, 2, 2, 3, 5, 1, 6 },
  { 2, 1, 13, 4, 4, 8, 4, 1, 10, 2, 1, 4, 2, 28, 7, 3, 6, 1, 3, 1, 4, 9, 1, 15, 3, 56, 2, 9, 2 },
  { 1, 10, 3, 1, 2, 18, 1, 551, 1, 8, 40, 21, 4, 4, 1, 4, 12, 4, 12, 64, 6, 30, 5, 4, 4, 2, 1, 12, 1 },
  { 10, 1, 24, 2, 16, 2, 24, 1, 7, 2, 1, 6, 1, 2, 18, 4, 1, 4, 1, 24, 2, 1, 20, 1, 95, 2, 21, 1, 14 },
  { 2, 8, 1, 42, 8, 1, 2, 36, 2, 2, 20, 3, 6, 3, 7, 6, 2, 1, 232, 2, 1, 20, 2, 9, 1, 108, 2, 4, 6 },
  { 1, 2, 3, 10, 1, 3, 14, 5, 1, 80, 119, 2, 24, 30, 1, 2, 10, 60, 9, 3, 56, 4, 1, 3, 2, 8, 2, 1, 3 },
  { 30, 1, 8, 1, 3, 9, 1, 2, 16, 14, 1, 12, 2, 2, 11, 36, 1, 174, 2, 1, 12, 7, 12, 3, 16, 24, 29, 69, 1 },
  { 1, 3, 1, 56, 2, 1, 24, 1, 3, 1, 32, 4, 1, 60, 4, 2, 9, 1, 15, 138, 2, 10, 1, 2, 3, 18, 1, 2, 17 },
  { 3, 15, 2, 1, 72, 2, 14, 4, 9, 3, 10, 1, 4, 6, 1, 3, 1, 12, 1, 24, 5, 1, 2, 6, 1, 6, 21, 1, 2 },
  { 2, 2, 6, 5, 8, 28, 4, 3, 1, 2, 16, 6, 1, 72, 2, 6, 2, 1, 6, 1, 26, 4, 76, 1, 21, 4, 1, 45, 8 },
  { 16, 1, 4, 2, 1, 6, 1, 57, 4, 4, 136, 4, 8, 8, 12, 80, 20, 12, 2, 2, 4, 7, 3, 2, 2, 8, 11, 2, 1 },
  { 1, 6, 42, 3, 121, 1, 2, 8, 2, 10, 2, 399, 38, 1, 330, 2, 1, 48, 8, 3, 3, 1, 10, 45, 2, 280, 1, 6, 2 },
  { 3, 2, 1, 5, 1, 6, 9, 4, 1, 16, 1, 2, 4, 6, 1, 17, 2, 22, 1, 10, 16, 9, 39, 6, 3, 18, 13, 9, 6 },
  { 4, 8, 2, 6, 10, 13, 44, 2, 5, 3, 120, 3, 1, 2, 24, 54, 8, 9, 7, 3, 1, 2, 6, 1, 5, 15, 1, 36, 3 },
  { 1, 16, 9, 28, 18, 9, 24, 1, 175, 2, 1, 24, 10, 1, 5, 3, 1, 2, 28, 1, 18, 14, 1, 3, 1, 8, 2, 3, 1 },
  { 9, 60, 1, 2, 12, 3, 4, 3, 8, 1, 16, 1, 2, 6, 3, 15, 3, 28, 1, 3, 6, 1, 10, 72, 16, 3, 3, 16, 2 },
  { 1, 10, 87, 435, 4, 2, 1, 10, 1, 24, 26, 12, 15, 2, 1, 8, 1, 2, 200, 100, 2, 8, 1, 2, 4, 1, 10, 4, 18 },
  { 2, 2, 48, 4, 2, 1, 6, 25, 2, 4, 42, 2, 20, 16, 5, 3, 4, 1, 12, 3, 2, 1, 17, 10, 1, 4, 6, 1, 24 },
  { 6, 1, 4, 1, 2, 13, 3, 2, 4, 7, 1, 2, 3, 638, 1, 12, 6, 3, 1, 4, 16, 4, 92, 12, 5, 40, 4, 3, 2 },
  { 3, 4, 1, 28, 15, 2, 2, 9, 1, 78, 2, 10, 2, 1, 12, 1, 22, 4, 308, 1, 2, 28, 1, 4, 2, 6, 1, 2, 18 },
  { 10, 14, 4, 1, 42, 4, 1, 2, 99, 9, 6, 1, 4, 2, 10, 36, 1, 6, 1, 11, 42, 7, 12, 3, 1, 2, 5, 5, 8 },
  { 25, 2, 1, 5, 2, 1, 14, 2, 2, 4, 2, 6, 48, 6, 3, 2, 377, 1, 12, 6, 2, 3, 21, 2, 12, 1, 2, 15, 1 },
  { 1, 6, 9, 2, 16, 4, 56, 4, 10, 3, 1, 2, 6, 18, 2, 17, 1, 308, 20, 4, 1, 16, 1, 168, 12, 8, 16, 22, 336 },
  { 4, 6, 1, 2, 3, 24, 3, 1, 2, 130, 12, 5, 1, 15, 2, 1, 4, 1, 8, 3, 8, 2, 11, 2, 1, 64, 1, 4, 15 },
  { 2, 1, 3, 15, 2, 10, 1, 10, 4, 2, 14, 1, 39, 1, 14, 841, 1, 3, 4, 2, 18, 1, 2, 7, 2, 4, 2, 2, 8 },
  { 2, 88, 21, 2, 4, 75, 3, 4, 2, 1, 20, 15, 1, 4, 1, 9, 15, 1, 70, 2, 1, 9, 2, 1, 24, 1, 28, 5, 1 },
  { 27, 3, 2, 1, 5, 2, 12, 3, 49, 4, 1, 2, 18, 1, 6, 4, 2, 6, 2, 14, 18, 1, 32, 6, 1, 5, 4, 2, 2 },
  { 1, 22, 2, 10, 7, 1, 6, 1, 4, 2, 2, 325, 3, 2, 1, 12, 4, 1, 3, 1, 174, 2, 1, 348, 10, 8, 2, 1, 6 }
};
  castleArea ( alt7, 29, result );
  assert ( identicalMap ( result, area7, 29 ) );

  static int altY[MAP_MAX][MAP_MAX];
  for (size_t i = 0; i < MAP_MAX; i ++)
  {
    for (size_t j = 0; j < MAP_MAX; j++) 
    {
      altY[i][j] = 1;
    }
  }
    altY[50][50] = 2;
  static int areaY[MAP_MAX][MAP_MAX];
  for (size_t i = 0; i < MAP_MAX; i ++)
  {
    for (size_t j = 0; j < MAP_MAX; j++) 
    {
      areaY[i][j] = 1;
    }
  }
  altY[50][50] = 2500;
  castleArea ( altY, MAP_MAX, result ); 
  assert ( identicalMap ( result, areaY, 50 ) );



  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
