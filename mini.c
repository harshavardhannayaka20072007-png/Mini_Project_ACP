#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define WIDTH 80
#define HEIGHT 25
#define MAX_OBJECTS 100

typedef enum {
    RECTANGLE,
    LINE,
    CIRCLE,
    TRIANGLE
} ShapeType;

typedef struct {
    int id;
    ShapeType type;

    union {
        struct {
            int x, y, w, h;
        } rect;

        struct {
            int x1, y1, x2, y2;
        } line;

        struct {
            int xc, yc, r;
        } circle;

        struct {
            int x1, y1, x2, y2, x3, y3;
        } triangle;
    } data;

} Shape;

char canvas[HEIGHT][WIDTH];
Shape objects[MAX_OBJECTS];
int objectCount = 0;
int nextId = 1;

void clearCanvas() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            canvas[i][j] = '_';
        }
    }
}

void putPixel(int x, int y) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
        canvas[y][x] = '*';
}

void drawLinePrimitive(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int err = dx - dy;

    while (1) {
        putPixel(x1, y1);

        if (x1 == x2 && y1 == y2)
            break;

        int e2 = 2 * err;

        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }

        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void drawRectanglePrimitive(int x, int y, int w, int h) {
    for (int i = x; i < x + w; i++) {
        putPixel(i, y);
        putPixel(i, y + h - 1);
    }

    for (int i = y; i < y + h; i++) {
        putPixel(x, i);
        putPixel(x + w - 1, i);
    }
}

void drawCirclePrimitive(int xc, int yc, int r) {
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    while (y >= x) {
        putPixel(xc + x, yc + y);
        putPixel(xc - x, yc + y);
        putPixel(xc + x, yc - y);
        putPixel(xc - x, yc - y);

        putPixel(xc + y, yc + x);
        putPixel(xc - y, yc + x);
        putPixel(xc + y, yc - x);
        putPixel(xc - y, yc - x);

        x++;

        if (d > 0) {
            y--;
            d += 4 * (x - y) + 10;
        } else {
            d += 4 * x + 6;
        }
    }
}

void drawTrianglePrimitive(
    int x1, int y1,
    int x2, int y2,
    int x3, int y3) {

    drawLinePrimitive(x1, y1, x2, y2);
    drawLinePrimitive(x2, y2, x3, y3);
    drawLinePrimitive(x3, y3, x1, y1);
}

void redrawCanvas() {
    clearCanvas();

    for (int i = 0; i < objectCount; i++) {
        Shape *s = &objects[i];

        switch (s->type) {
            case RECTANGLE:
                drawRectanglePrimitive(
                    s->data.rect.x,
                    s->data.rect.y,
                    s->data.rect.w,
                    s->data.rect.h
                );
                break;

            case LINE:
                drawLinePrimitive(
                    s->data.line.x1,
                    s->data.line.y1,
                    s->data.line.x2,
                    s->data.line.y2
                );
                break;

            case CIRCLE:
                drawCirclePrimitive(
                    s->data.circle.xc,
                    s->data.circle.yc,
                    s->data.circle.r
                );
                break;

            case TRIANGLE:
                drawTrianglePrimitive(
                    s->data.triangle.x1,
                    s->data.triangle.y1,
                    s->data.triangle.x2,
                    s->data.triangle.y2,
                    s->data.triangle.x3,
                    s->data.triangle.y3
                );
                break;
        }
    }
}

void displayCanvas() {
    redrawCanvas();

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%c", canvas[i][j]);
        }
        printf("\n");
    }
}

void addRectangle() {
    Shape s;
    s.id = nextId++;
    s.type = RECTANGLE;

    printf("x y width height: ");
    scanf("%d %d %d %d",
          &s.data.rect.x,
          &s.data.rect.y,
          &s.data.rect.w,
          &s.data.rect.h);

    objects[objectCount++] = s;

    printf("Rectangle ID = %d\n", s.id);
}

void addLine() {
    Shape s;
    s.id = nextId++;
    s.type = LINE;

    printf("x1 y1 x2 y2: ");
    scanf("%d %d %d %d",
          &s.data.line.x1,
          &s.data.line.y1,
          &s.data.line.x2,
          &s.data.line.y2);

    objects[objectCount++] = s;

    printf("Line ID = %d\n", s.id);
}

void addCircle() {
    Shape s;
    s.id = nextId++;
    s.type = CIRCLE;

    printf("centerX centerY radius: ");
    scanf("%d %d %d",
          &s.data.circle.xc,
          &s.data.circle.yc,
          &s.data.circle.r);

    objects[objectCount++] = s;

    printf("Circle ID = %d\n", s.id);
}

void addTriangle() {
    Shape s;
    s.id = nextId++;
    s.type = TRIANGLE;

    printf("x1 y1 x2 y2 x3 y3: ");
    scanf("%d %d %d %d %d %d",
          &s.data.triangle.x1,
          &s.data.triangle.y1,
          &s.data.triangle.x2,
          &s.data.triangle.y2,
          &s.data.triangle.x3,
          &s.data.triangle.y3);

    objects[objectCount++] = s;

    printf("Triangle ID = %d\n", s.id);
}

int findObject(int id) {
    for (int i = 0; i < objectCount; i++) {
        if (objects[i].id == id)
            return i;
    }
    return -1;
}

void deleteObject() {
    int id;

    printf("Enter ID to delete: ");
    scanf("%d", &id);

    int pos = findObject(id);

    if (pos == -1) {
        printf("Object not found\n");
        return;
    }

    for (int i = pos; i < objectCount - 1; i++) {
        objects[i] = objects[i + 1];
    }

    objectCount--;

    printf("Deleted\n");
}

void modifyObject() {
    int id;

    printf("Enter ID to modify: ");
    scanf("%d", &id);

    int pos = findObject(id);

    if (pos == -1) {
        printf("Object not found\n");
        return;
    }

    Shape *s = &objects[pos];

    switch (s->type) {
        case RECTANGLE:
            printf("New x y width height: ");
            scanf("%d %d %d %d",
                  &s->data.rect.x,
                  &s->data.rect.y,
                  &s->data.rect.w,
                  &s->data.rect.h);
            break;

        case LINE:
            printf("New x1 y1 x2 y2: ");
            scanf("%d %d %d %d",
                  &s->data.line.x1,
                  &s->data.line.y1,
                  &s->data.line.x2,
                  &s->data.line.y2);
            break;

        case CIRCLE:
            printf("New centerX centerY radius: ");
            scanf("%d %d %d",
                  &s->data.circle.xc,
                  &s->data.circle.yc,
                  &s->data.circle.r);
            break;

        case TRIANGLE:
            printf("New x1 y1 x2 y2 x3 y3: ");
            scanf("%d %d %d %d %d %d",
                  &s->data.triangle.x1,
                  &s->data.triangle.y1,
                  &s->data.triangle.x2,
                  &s->data.triangle.y2,
                  &s->data.triangle.x3,
                  &s->data.triangle.y3);
            break;
    }

    printf("Modified\n");
}

void listObjects() {
    printf("\nObjects:\n");

    for (int i = 0; i < objectCount; i++) {
        printf("ID %d : ", objects[i].id);

        switch (objects[i].type) {
            case RECTANGLE:
                printf("Rectangle\n");
                break;
            case LINE:
                printf("Line\n");
                break;
            case CIRCLE:
                printf("Circle\n");
                break;
            case TRIANGLE:
                printf("Triangle\n");
                break;
        }
    }
}

int main() {
    int choice;

    clearCanvas();

    while (1) {
        printf("\n===== 2D GRAPHICS EDITOR =====\n");
        printf("1. Add Rectangle\n");
        printf("2. Add Line\n");
        printf("3. Add Circle\n");
        printf("4. Add Triangle\n");
        printf("5. Delete Object\n");
        printf("6. Modify Object\n");
        printf("7. Display Picture\n");
        printf("8. List Objects\n");
        printf("9. Exit\n");
        printf("Choice: ");

        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addRectangle();
                break;

            case 2:
                addLine();
                break;

            case 3:
                addCircle();
                break;

            case 4:
                addTriangle();
                break;

            case 5:
                deleteObject();
                break;

            case 6:
                modifyObject();
                break;

            case 7:
                displayCanvas();
                break;

            case 8:
                listObjects();
                break;

            case 9:
                return 0;

            default:
                printf("Invalid choice\n");
        }
    }

    return 0;
}
