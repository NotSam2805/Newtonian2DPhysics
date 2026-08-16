#include "Renderer.hpp"

namespace n2p{
    
    // SDL_RenderDrawCircle and SDL_RenderFillCircle are from https://gist.github.com/Gumichan01/332c26f6197a432db91cc4327fcabb1c
    int SDL_RenderDrawCircle(SDL_Renderer * renderer, int x, int y, int radius) {
        int offsetx, offsety, d;
        int status;

        offsetx = 0;
        offsety = radius;
        d = radius -1;
        status = 0;

        while (offsety >= offsetx) {
            status += SDL_RenderDrawPoint(renderer, x + offsetx, y + offsety);
            status += SDL_RenderDrawPoint(renderer, x + offsety, y + offsetx);
            status += SDL_RenderDrawPoint(renderer, x - offsetx, y + offsety);
            status += SDL_RenderDrawPoint(renderer, x - offsety, y + offsetx);
            status += SDL_RenderDrawPoint(renderer, x + offsetx, y - offsety);
            status += SDL_RenderDrawPoint(renderer, x + offsety, y - offsetx);
            status += SDL_RenderDrawPoint(renderer, x - offsetx, y - offsety);
            status += SDL_RenderDrawPoint(renderer, x - offsety, y - offsetx);

            if (status < 0) {
                status = -1;
                break;
            }

            if (d >= 2*offsetx) {
                d -= 2*offsetx + 1;
                offsetx +=1;
            }
            else if (d < 2 * (radius - offsety)) {
                d += 2 * offsety - 1;
                offsety -= 1;
            }
            else {
                d += 2 * (offsety - offsetx - 1);
                offsety -= 1;
                offsetx += 1;
            }
        }

        return status;
    }

    int SDL_RenderFillCircle(SDL_Renderer * renderer, int x, int y, int radius) {
        int offsetx, offsety, d;
        int status;

        offsetx = 0;
        offsety = radius;
        d = radius -1;
        status = 0;

        while (offsety >= offsetx) {

            status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx,
                                         x + offsety, y + offsetx);
            status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety,
                                         x + offsetx, y + offsety);
            status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety,
                                         x + offsetx, y - offsety);
            status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx,
                                         x + offsety, y - offsetx);

            if (status < 0) {
                status = -1;
                break;
            }

            if (d >= 2*offsetx) {
                d -= 2*offsetx + 1;
                offsetx +=1;
            }
            else if (d < 2 * (radius - offsety)) {
                d += 2 * offsety - 1;
                offsety -= 1;
            }
            else {
                d += 2 * (offsety - offsetx - 1);
                offsety -= 1;
                offsetx += 1;
            }
        }

        return status;
    }


    void Renderer::DrawBody(const Rigidbody& body){
        const Shape* shape = body.GetShape();

        switch (shape->GetType()){
            case ShapeType::circle:
                DrawCircle(body);
                break;

            case ShapeType::rect:
                DrawRect(body);
                break;
            
            case ShapeType::polygon:
                DrawPolygon(body);
                break;
            
            default:
                throw "Unrecognised shape cannot be drawn";
                break;
        }
    }

    void Renderer::DrawCircle(const Rigidbody& circleBody){
        const Circle* shape = static_cast<const Circle*>(circleBody.GetShape());

        Vector2 centre = WorldToScreen(circleBody.GetPosition());
        float radius = shape->GetRadius() * camera->zoom;
        
        SDL_SetRenderDrawColor(renderer, shape->colour.red, shape->colour.green, shape->colour.blue, shape->colour.alpha);
        SDL_RenderFillCircle(renderer, centre.x, centre.y, radius);
    }

    void Renderer::DrawRect(const Rigidbody& rectBody){
        const Rect* shape = static_cast<const Rect*>(rectBody.GetShape());

        Vector2 screenPos = WorldToScreen(rectBody.GetPosition());

        SDL_Rect renderRect;
        SDL_SetRenderDrawColor(renderer, shape->colour.red, shape->colour.green, shape->colour.blue, shape->colour.alpha);
        renderRect.x = screenPos.x - (shape->GetWidth() * camera->zoom * 0.5f); renderRect.y = screenPos.y - (shape->GetHeight() * camera->zoom * 0.5f); // renderRect is drawn from top left, not centre
        renderRect.w = shape->GetWidth() * camera->zoom;
        renderRect.h = shape->GetHeight() * camera->zoom;
        SDL_RenderFillRect(renderer, &renderRect);
    }

    void Renderer::DrawPolygon(const Rigidbody& polyBody){
        const Polygon* shape = static_cast<const Polygon*>(polyBody.GetShape());

        const Transform& transform = polyBody.GetTransform();

        SDL_SetRenderDrawColor(renderer, shape->colour.red, shape->colour.green, shape->colour.blue, shape->colour.alpha);

        for (size_t i = 0; i < shape->GetVertexCount(); ++i){
            Vector2 aWorld = shape->GetWorldVertex(i, transform);// Bug on this line
            Vector2 bWorld = shape->GetWorldVertex((i + 1) % int(shape->GetVertexCount()), transform);

            Vector2 a = WorldToScreen(aWorld);
            Vector2 b = WorldToScreen(bWorld);
            
            SDL_RenderDrawLine(renderer, a.x, a.y, b.x, b.y);
        }
    }

    Renderer::Renderer(int windowWidth, int windowHeight, Camera camera, Colour backgroundColour) : camera(&camera), backgroundColour(backgroundColour) {
        window = SDL_CreateWindow("Newtonian 2D Physics", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);
        renderer = SDL_CreateRenderer(window, -1, 0);
    }

    void Renderer::Clear(){
        SDL_SetRenderDrawColor(renderer, backgroundColour.red, backgroundColour.green, backgroundColour.blue, backgroundColour.alpha); // Set to background colour
        SDL_RenderClear(renderer);
    }

    void Renderer::DrawWorld(const PhysicsWorld& world){
        for (const auto& body : world.GetBodies()){
            DrawBody(*body);
        }
    }

    void Renderer::Present(){
        SDL_RenderPresent(renderer);
    }

    bool Renderer::IsInScreenBounds(const Vector2& screenPosition){
        
        if (screenPosition.x < 0.0f | screenPosition.y < 0.0f){
            return false;
        }

        int width; int height;
        SDL_GetWindowSize(window, &width, &height);

        return !(screenPosition.x > width | screenPosition.y > height);
    }

    Vector2 Renderer::WorldToScreen(const Vector2& world){
        int width; int height;
        SDL_GetWindowSize(window, &width, &height);

        Vector2 relative = world - camera->position;

        return Vector2(
            width * 0.5f + relative.x * camera->zoom,
            height * 0.5f - relative.y * camera->zoom
        );
    }

    void Renderer::Quit(){
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
    }
}