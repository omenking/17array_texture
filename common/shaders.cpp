#include "shaders.h"

GLuint load_shaders( char *vert, char *frag )
{
	GLuint program;
	GLuint vertex   = glCreateShader(GL_VERTEX_SHADER);	
	GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);

	GLint status;
	int log_length;

  const char* text;
	string program_text;

	// Compile Vertex Shader
	text         = get_shader(vert, program_text);
	GLint length = program_text.size();
	glShaderSource(vertex, 1, &text, NULL);
	glCompileShader(vertex);

	// Check Vertex Shader
	glGetShaderiv(vertex, GL_COMPILE_STATUS , &status);
	glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &log_length);
	if ( !( status == GL_TRUE ) )
		cerr << "\nVertex Shader compilation failed..." << '\n';
	if (log_length > 0){
		std::vector<char> VertexShaderErrorMessage(log_length+1);
		glGetShaderInfoLog(vertex, log_length, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Vertex Fragment
	program_text = "";
	text = get_shader(frag, program_text);
	glShaderSource(fragment, 1, &text, NULL);
	glCompileShader(fragment);

	// Check Vertex Fragment
	glGetShaderiv(fragment, GL_COMPILE_STATUS , &status);
	glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &log_length);
	if ( !( status == GL_TRUE ) )
		cerr << "\nFragment Shader compilation failed..." << '\n';
	if ( log_length > 0 ){
		std::vector<char> FragmentShaderErrorMessage(log_length+1);
		glGetShaderInfoLog(fragment, log_length, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link Shader Program
	program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);

  // Check The Shader Program
	glGetProgramiv(program, GL_LINK_STATUS    , &status );
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
	if ( !( status == GL_TRUE ) )
		cout << "Link failed..." << endl;
	if ( log_length > 0 ){
		std::vector<char> ProgramErrorMessage(log_length+1);
		glGetProgramInfoLog(program, log_length, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

  glDeleteShader(vertex);
  glDeleteShader(fragment);

	return program;
}

const char* get_shader( const char *filePath, string &shader )
{
	fstream shaderFile( filePath, ios::in );

	if ( shaderFile.is_open() )
	{
		std::stringstream buffer;
		buffer << shaderFile.rdbuf();
		shader = buffer.str();
		buffer.clear();
	}
	shaderFile.close();

	return shader.c_str();
}
