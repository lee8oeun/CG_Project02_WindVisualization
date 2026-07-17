#include <iostream>
#include <cstdlib>
#include <ctime>
#include <Wind.h>
#include <random>

Wind::Wind() {
    vao, ssbo = 0;
    c_shader = nullptr;
}

void Wind::initialize()
{
    //지구 중심 위치, 반지름 크기 
    glm::vec3 Center = glm::vec3(0.0f, 0.0f, 0.0f);
    float radius = 1.0f;
    glm::vec3 normal = glm::vec3(1.0f,1.0f,1.0f);

    glm::vec3 Position = Center + glm::normalize(normal)*radius*1.0f;
    
    particles.resize(1);

    particles[0].position[0] = Position.x;
    particles[0].position[1] = Position.y;
    particles[0].position[2] = Position.z;


    //vao, ssbo 생성
    glCreateVertexArrays(1, &vao);
    glCreateBuffers(1, &ssbo);
    glNamedBufferData(
        ssbo,
        sizeof(Particle) * particles.size(),
        particles.data(),
        GL_DYNAMIC_DRAW
    );
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo); //이 버퍼를 binding0번에 연결!
    glVertexArrayVertexBuffer(
        vao,
        0,
        ssbo,
        0,
        sizeof(Particle)
    );
    glVertexArrayAttribFormat(
        vao,
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        0
    );  // position xyz
    glVertexArrayAttribBinding(vao, 0, 0);
    glEnableVertexArrayAttrib(vao, 0);
    

    //compute shader 로드
    c_shader = new ComputeShader("shaders/wind.comp");
    glEnable(GL_PROGRAM_POINT_SIZE);    //gl_point를 사용하기 위해 
    // 렌더링 루프 내부 혹은 설정 부분에 작성

    
}


void Wind::draw() {
    glBindVertexArray(vao);
    
    glDrawArrays(GL_POINTS, 0, 1);
    glBindVertexArray(0);
}