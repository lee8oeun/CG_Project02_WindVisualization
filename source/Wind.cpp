#include <iostream>
#include <cstdlib>
#include <ctime>
#include <Wind.h>
#include <random>

//min~max 사이의 랜덤한 실수 값 생성
float randomFloat(float min, float max) {
    static random_device rd;
    static mt19937 gen(rd());

    uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

vector<glm::vec3> generateNormals(int count) {
    vector<glm::vec3> normals;
    normals.reserve(count);

    //normal벡터 파티클 수 만큼 생성 
    for (int i = 0;i < count;i++) {
        float x = randomFloat(-1.0f, 1.0f);
        float y = randomFloat(-1.0f, 1.0f);
        float z = randomFloat(-1.0f, 1.0f);

        glm::vec3 dir = glm::vec3(x, y, z);
        glm::vec3 normal = glm::normalize(dir);

        normals.push_back(normal);
    }
    return normals;
}

WindParticles::WindParticles()
    : vao(0), ssbo(0), c_shader(nullptr) {
}

void WindParticles::initialize()
{
    //particle위치 생성
    glm::vec3 Center = glm::vec3(0.0f, 0.0f, 0.0f);
    float Radius = 1.02f;

    particles.resize(NUM_PARTICLES);

    vector<glm::vec3> normals = generateNormals(NUM_PARTICLES);
    
        for (int i = 0; i < NUM_PARTICLES; i++)
        {
            glm::vec3 normal = normals[i];

            glm::vec3 pos = Center + normal * Radius;
            particles[i].position[0] = pos.x;
            particles[i].position[1] = pos.y;
            particles[i].position[2] = pos.z;
            particles[i].position[3] = 1.0f;

            particles[i].velocity[0] = 0.0f;
            particles[i].velocity[1] = 0.0f;
            particles[i].velocity[2] = 0.0f;
            particles[i].velocity[3] = 0.0f;
        }


    //ssbo : 컴퓨트 쉐이더용
    glCreateBuffers(1, &ssbo);
    glNamedBufferData( //gpu한테 데이터 복사 
        ssbo,
        sizeof(Particle) * particles.size(),  //파티클 하나*파티클 개수 
        particles.data(), //파티클 시작 주소 
        GL_DYNAMIC_DRAW   //이 데이터를 자주 수정할 예정 이라는 힌트!!
    );
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo); //이 버퍼를 binding0번에 연결!


    //vao: 버텍스 버퍼용
    glCreateVertexArrays(1, &vao);
    glVertexArrayVertexBuffer(
        vao,
        0,
        ssbo,  //기존 ssbo 버퍼를 버텍스 버퍼와 연결 
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
    glEnable(GL_PROGRAM_POINT_SIZE);
}

void WindParticles::update(float dt) {
    c_shader->use();
    c_shader->setFloat("dt", dt);

    glDispatchCompute(NUM_PARTICLES / 256, 1, 1);   //compute shader 실행
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT); //ssbo의 결과를 동기화 
}


void WindParticles::draw() {
    glBindVertexArray(vao);

    glDrawArrays(GL_POINTS, 0, NUM_PARTICLES);
    glBindVertexArray(0);
}