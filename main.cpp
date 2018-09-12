#include "game.hpp"
#include "Error.hpp"

int main()
{
  Game game;

  try
  {
    game.run();
  }
  catch(Error& e)
  {
    std::cerr << e.info() << std::endl;
  }

  return 0;
}
