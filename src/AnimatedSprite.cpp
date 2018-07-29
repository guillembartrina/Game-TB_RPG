#include "AnimatedSprite.hpp"

AnimatedSprite::AnimatedSprite() : sf::Sprite()
{
    _currentAnimation = nullptr;
    _currentFrame = 0;
    _currentTime = sf::Time::Zero;
    _playing = false;
}

AnimatedSprite::AnimatedSprite(const AnimatedSprite& original) : sf::Sprite()
{
    _animations = original._animations;
    if(original._currentAnimation == nullptr)
    {
        _currentAnimation = nullptr;
        _currentFrame = 0;
    }
    else
    {
        setActiveAnimation(original._currentAnimation->name);
        setAnimationFrame(original._currentFrame);
    }
    _currentTime = sf::Time::Zero;
    _playing = _playing;
}

AnimatedSprite::~AnimatedSprite()
{
    _animations.clear();
}

void AnimatedSprite::addAnimation()
{
    Animation animation;
    _animations.insert(std::make_pair("NULL", animation));
}

void AnimatedSprite::addAnimation(const std::string& name, const sf::Texture& texture, unsigned int numFrames, sf::Vector2u size, const sf::Time& frameTime, bool repetition)
{
    std::pair<std::map<std::string, Animation>::iterator, bool> it = _animations.insert(std::make_pair(name, Animation()));

    if(!it.second) std::cerr << "Already exist an animation with the same name <" << name << ">." << std::endl;
    else
    {
        _currentAnimation = &it.first->second;

        setAnimationName(name);
        setAnimationTexture(texture);
        setAnimationFrames(numFrames, size);
        setAnimationFrameTime(frameTime);
        setAnimationRepetition(repetition);

        _currentAnimation = nullptr;
    }
}

void AnimatedSprite::addAnimation(const std::string& name, const sf::Texture& texture, unsigned int numFrames, unsigned int initRow, sf::Vector2u size, const sf::Time& frameTime, bool repetition)
{
    std::pair<std::map<std::string, Animation>::iterator, bool> it = _animations.insert(std::make_pair(name, Animation()));

    if(!it.second) std::cerr << "Already exist an animation with the same name <" << name << ">." << std::endl;
    else
    {
        _currentAnimation = &it.first->second;

        setAnimationName(name);
        setAnimationTexture(texture);
        setAnimationFrames(numFrames, initRow, size);
        setAnimationFrameTime(frameTime);
        setAnimationRepetition(repetition);

        _currentAnimation = nullptr;
    }
}

void AnimatedSprite::addAnimation(const std::string& name, const sf::Texture& texture, std::vector<sf::IntRect> frames, const sf::Time& frameTime, bool repetition)
{

    std::pair<std::map<std::string, Animation>::iterator, bool> it = _animations.insert(std::make_pair(name, Animation()));

    if(!it.second) std::cerr << "Already exist an animation with the same name <" << name << ">." << std::endl;
    else
    {
        _currentAnimation = &it.first->second;

        setAnimationName(name);
        setAnimationTexture(texture);
        setAnimationFrames(frames);
        setAnimationFrameTime(frameTime);
        setAnimationRepetition(repetition);

        _currentAnimation = nullptr;
    }
}

void AnimatedSprite::setAnimationName(std::string name)
{
    if(_currentAnimation != nullptr)
    {
        _currentAnimation->name = name;
    }
}

void AnimatedSprite::setAnimationTexture(const sf::Texture& texture)
{
    if(_currentAnimation != nullptr)
    {
        _currentAnimation->texture = &texture;
    }
}

void AnimatedSprite::setAnimationFrameTime(const sf::Time& frameTime)
{
    if(_currentAnimation != nullptr)
    {
        _currentAnimation->frameTime = frameTime;
    }
}

void AnimatedSprite::setAnimationFrames(unsigned int numFrames, sf::Vector2u size)
{
    if(_currentAnimation != nullptr)
    {
        for(unsigned int i = 0; i < numFrames; ++i)
        {
            unsigned int textureW = _currentAnimation->texture->getSize().x;
            int posX = (i * size.x) % (textureW);
            int posY = int((i * size.x) / (textureW));
            _currentAnimation->textureRects.push_back(sf::IntRect(posX, posY, size.x, size.y));
        }
    }
}

void AnimatedSprite::setAnimationFrames(unsigned int numFrames, unsigned int initRow, sf::Vector2u size)
{
    if(_currentAnimation != nullptr)
    {
        for(unsigned int i = 0; i < numFrames; ++i)
        {
            unsigned int textureW = _currentAnimation->texture->getSize().x;
            int posX = (i * size.x) % (textureW);
            int posY = (initRow * size.y) + int((i * size.x) / (textureW));
            _currentAnimation->textureRects.push_back(sf::IntRect(posX, posY, size.x, size.y));
        }
    }
}

void AnimatedSprite::setAnimationFrames(std::vector<sf::IntRect> frames)
{
    if(_currentAnimation != nullptr)
    {
        _currentAnimation->textureRects = frames;
    }
}

void AnimatedSprite::setAnimationRepetition(bool repetition)
{
    if(_currentAnimation != nullptr)
    {
        _currentAnimation->repetition = repetition;
    }
}

void AnimatedSprite::setActiveAnimation(const std::string& name, bool play)
{
    std::map<std::string, Animation>::iterator it = _animations.find(name);

    if(!_animations.empty() && it != _animations.end())
    {
        _currentAnimation = &_animations.at(name);
        setTexture(*_currentAnimation->texture);
        _currentTime = sf::Time::Zero;
        _playing = play;
        firstAnimationFrame();
    }
    else
    {
        std::cerr << "No animation of index <" << name <<">." << std::endl;
    }
}

void AnimatedSprite::playAnimation()
{
    _playing = true;
}

void AnimatedSprite::pauseAnimation()
{
    _playing = false;
}

void AnimatedSprite::stopAnimation()
{
    firstAnimationFrame();
    _playing = false;
}

void AnimatedSprite::firstAnimationFrame()
{
    if(_currentAnimation != nullptr)
    {
        if(!_currentAnimation->textureRects.empty())
        {
            _currentFrame = 0;
            setTextureRect(_currentAnimation->textureRects[_currentFrame]);
        }
    }
}

void AnimatedSprite::nextAnimationFrame()
{
    if(_currentAnimation != nullptr)
    {
        if(!_currentAnimation->textureRects.empty())
        {
            if((_currentAnimation->repetition) || !(_currentFrame == _currentAnimation->textureRects.size()-1))
            {
                _currentFrame = (_currentFrame + 1) % _currentAnimation->textureRects.size();
            }
            setTextureRect(_currentAnimation->textureRects[_currentFrame]);
        }
    }
}

void AnimatedSprite::setAnimationFrame(unsigned int frame)
{
    if(_currentAnimation != nullptr)
    {
        if(!_currentAnimation->textureRects.empty() && frame < _currentAnimation->textureRects.size())
        {
            _currentFrame = frame;
            setTextureRect(_currentAnimation->textureRects[_currentFrame]);
        }
        else
        {
            std::cerr << "No frame of index <" << frame << ">." << std::endl;
        }
    }
}

void AnimatedSprite::updateAnimation(const sf::Time& deltatime)
{
    if(_currentAnimation != nullptr)
    {
        if(_playing)
        {
            _currentTime += deltatime;
            if(_currentTime >= _currentAnimation->frameTime)
            {
                nextAnimationFrame();
                _currentTime -= _currentAnimation->frameTime;
            }
        }
    }
}

bool AnimatedSprite::isAnimationPlaying()
{
    return _playing;
}

bool AnimatedSprite::hasAnimationEnded()
{
    if(_currentAnimation != nullptr)
    {
        if((!_currentAnimation->repetition) && (_currentFrame == _currentAnimation->textureRects.size()-1))
        {
            return true;
        }
    }
    
    return false;
}