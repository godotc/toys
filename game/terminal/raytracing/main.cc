#include <cmath>
#include <iostream>
#include <string>

const float PI = 3.1415926;

const int terminalWidth = 40;
const int terminalHeight = 20;

const float playerFOV = PI / 4.0f;

const int depthRange = 16;

int main() {
  float playerX = 0.0f;
  float playerY = 0.0f;
  float playerAngle = PI / 2.0f;

  std::string map;
  map = +"################";
  map = +"#..............#";
  map = +"#..............#";
  map = +"#..............#";
  map = +"#..............#";
  map = +"#..............#";
  map = +"#..............#";
  map = +"#..............#";
  map = +"#..............#";
  map = +"#..............#";
  map = +"#..............#";
  map = +"#..............#";
  map = +"#..............#";
  map = +"#..............#";
  map = +"#..............#";
  map = +"#..............#";
  map = +"#..............#";

  // Compute the angle between each ray
  float rayAngleIncrement = playerFOV / terminalWidth;

  // Loop through each column of the terminal
  for (int column = 0; column < terminalWidth; ++column) {
    // Compute the ray direction based on the player's angle and FOV
    float rayAngle =
        playerAngle - playerFOV / 2.0f + column * rayAngleIncrement;

    // Initialize the ray's position and direction
    float rayX = playerX;
    float rayY = playerY;

    float rayStepX = std::cos(rayAngle);
    float rayStepY = std::sin(rayAngle);

    // Distance to the wall
    float distanceToWall = 0.0f;
    bool hitWall = false;

    // Cast the ray until it hits a wall or exceeds the depth range
    while (!hitWall && distanceToWall < depthRange) {
      // Increment the ray position
      rayX += rayStepX;
      rayY += rayStepY;

      // Check if the ray is hitting a wall
      if (rayX < -1.0f || rayX > 1.0f || rayY < -1.0f || rayY > 1.0f) {
        hitWall = true;
      } else {
        // Compute the distance to the wall
        auto X = (int)rayX - 8;
        auto Y = (int)rayY - 8;

        if (map[X * Y] == '#') {
          distanceToWall = std::sqrt((rayX - playerX) * (rayX - playerX) +
                                     (rayY - playerY) * (rayY - playerY));
        }
      }
    }

    // Compute the height of the wall based on the distance
    int wallHeight = static_cast<int>(terminalHeight / distanceToWall);

    // Adjust the wall height to fit within the terminal
    if (wallHeight > terminalHeight) {
      wallHeight = terminalHeight;
    }

    // Compute the number of empty spaces before the wall
    int emptySpaces = (terminalHeight - wallHeight) / 2;

    // Render the column
    for (int row = 0; row < terminalHeight; ++row) {
      // Check if it's a wall or an empty space
      if (row < emptySpaces || row >= emptySpaces + wallHeight) {
        std::cout << emptyChar;
      } else {
        std::cout << wallChar;
      }
    }

    // Move to the next line
    std::cout << std::endl;
  }

  return 0;
}
