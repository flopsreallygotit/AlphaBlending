#include "mainUtils.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int main ()
{
    image_t *Background = imageConstructor("Data/TableResize.bmp");
    image_t *Foreground = imageConstructor("Data/AskhatCatResize.bmp");

    sf::Clock clock;
    unsigned char *result = (unsigned char *) alphaBlending(Background, Foreground);
    printf(BOLD MAGENTA "Time: %f sec.\n" RESET, clock.getElapsedTime().asSeconds());

    imageDestructor(Foreground);

    sf::Image image;
    image.create(Background->width, Background->height, result);

    sf::Texture texture;
    texture.loadFromImage(image);

    sf::Sprite sprite;
    sprite.setTexture(texture);

    sf::RenderWindow window(sf::VideoMode(Background->width, Background->height), "Alpha Blending");

    imageDestructor(Background);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        window.clear();
        window.draw(sprite);
        window.display();
    }

    free(result);

    return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
