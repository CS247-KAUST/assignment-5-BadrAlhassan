// CS 247 - Scientific Visualization, KAUST
//
// Programming Assignment #5
#include <cstring>
#include "CS247_prog.h"

// cycle clear colors
static void nextClearColor()
{
    clearColor = (++clearColor) % 3;

    switch(clearColor)
    {
        case 0:
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            break;
        case 1:
            glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
            break;
        default:
            glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
            break;
    }
}


// callbacks
// framebuffer to fix viewport
void frameBufferCallback(GLFWwindow* window, int width, int height)
{
    view_width = width;
    view_height = height;
    glViewport(0, 0, width, height);
}

// key callback to take user inputs for both windows
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_RELEASE) {
        char* status[ 2 ];
        status[ 0 ] = "disabled";
        status[ 1 ] = "enabled";

        switch (key) {
            case '1':
                toggle_xy = 0;
                LoadData( filenames[ 0 ] );
                loaded_file = 0;
                fprintf( stderr, "Loading " );
                fprintf( stderr, filenames[ 0 ] );
                fprintf( stderr, " dataset.\n");
                break;
            case '2':
                toggle_xy = 0;
                LoadData(filenames[ 1 ] );
                loaded_file = 1;
                fprintf( stderr, "Loading " );
                fprintf( stderr, filenames[ 1 ] );
                fprintf( stderr, " dataset.\n");
                break;
            case '3':
                toggle_xy = 1;
                LoadData( filenames[ 2 ] );
                loaded_file = 2;
                fprintf( stderr, "Loading " );
                fprintf( stderr, filenames[ 2 ] );
                fprintf( stderr, " dataset.\n");
                break;
            case '0':
                if( num_timesteps > 1 ){
                    loadNextTimestep();
                    fprintf( stderr, "Timestep %d.\n", loaded_timestep );
                }
                break;
            case GLFW_KEY_A:
                en_arrow = !en_arrow;
                fprintf(stderr, "%s drawing arrows.\n", en_arrow? "enabling" : "disabling");
                break;
            case GLFW_KEY_S:
                current_scalar_field = (current_scalar_field + 1)%num_scalar_fields;
                DownloadScalarFieldAsTexture();
                fprintf( stderr, "Scalar field changed.\n");
                break;
            case GLFW_KEY_B:
                nextClearColor();
                fprintf( stderr, "Next clear color.\n");
                break;
            case GLFW_KEY_EQUAL:
                sampling_rate = std::min(sampling_rate + 5, 100);
                fprintf(stderr, "Increasing sampling rate to %d.\n", sampling_rate);
                break;
            case GLFW_KEY_MINUS:
                sampling_rate = std::max(sampling_rate - 5, 5);
                fprintf(stderr, "Decreasing sampling rate to: %d.\n", sampling_rate);
                break;
            case GLFW_KEY_I:
                dt = std::min(dt + 0.005, 1.);
                fprintf(stderr, "Increase dt: %.2f\n", dt);
                break;
            case GLFW_KEY_K:
                dt = std::max(dt - 0.005, 0.0001);
                fprintf(stderr, "Decrease dt: %.2f\n", dt);
                break;
            case GLFW_KEY_T:
                en_streamline = !en_streamline;
                fprintf(stderr, "%s drawing streamlines.\n", en_streamline? "enabling" : "disabling");
                break;
            case GLFW_KEY_P:
                en_pathline = !en_pathline;
                fprintf(stderr, "%s drawing pathlines.\n", en_pathline? "enabling" : "disabling");
                break;
            // TODO: add keyboard controls for:
            //   - toggle colormap mode (cycle off/rainbow/cool-warm)
            //   - adjust blend factor (increase/decrease between 0.0 and 1.0)
            //   - toggle integration method (Euler/RK2)
            //   - clear all seeds
            case GLFW_KEY_C:
                colormap_mode = (colormap_mode + 1) % 3;
                fprintf(stderr, "Colormap: %s\n",
                    colormap_mode == 0 ? "off" :
                    colormap_mode == 1 ? "rainbow" : "cool-warm");
                break;
            case GLFW_KEY_V:
                blend_factor = std::min(blend_factor + 0.1f, 1.0f);
                fprintf(stderr, "Blend: %.2f\n", blend_factor);
                break;
            case GLFW_KEY_N:
                blend_factor = std::max(blend_factor - 0.1f, 0.0f);
                fprintf(stderr, "Blend: %.2f\n", blend_factor);
                break;
            case GLFW_KEY_R:
                integration_method = (integration_method + 1) % 3;
                fprintf(stderr, "Integration: %s\n",
                    integration_method == 0 ? "Euler" :
                    integration_method == 1 ? "RK2" : "RK4");
                break;
            case GLFW_KEY_L:
                arrow_length_mode = !arrow_length_mode;
                fprintf(stderr, "Arrow length: %s\n", arrow_length_mode ? "speed" : "constant");
                break;
            case GLFW_KEY_G:
                rake_mode = (rake_mode + 1) % 3;
                fprintf(stderr, "Rake mode: %s\n",
                    rake_mode == 0 ? "off" :
                    rake_mode == 1 ? "horizontal" : "vertical");
                break;
            case GLFW_KEY_X:
                streamlines.clear(); streamline_seeds.clear();
                pathlines.clear(); pathline_seeds.clear();
                fprintf(stderr, "Cleared all seeds.\n");
                break;
            case GLFW_KEY_Q:
            case GLFW_KEY_ESCAPE:
                exit( 0 );
                break;
            default:
                fprintf( stderr, "\nKeyboard commands:\n\n"
                                 "1, load %s dataset\n"
                                 "2, load %s dataset\n"
                                 "3, load %s dataset\n"
                                 "0, cycle through timesteps\n"
                                 "b, switch backgropund color\n"
                                 "a, en-/disable arrows.\n"
                                 "t, en-/disable streamlines.\n"
                                 "p, en-/disable pathlines.\n"
                                 "+, increase sampling rate.\n"
                                 "-, decrease sampling rate.\n"
                                 "i, increase dt.\n"
                                 "k, decrease dt.\n"
                                 "c, cycle colormap (off/rainbow/cool-warm).\n"
                                 "v, increase blend factor.\n"
                                 "n, decrease blend factor.\n"
                                  "r, toggle integration (Euler/RK2/RK4).\n"
                                 "l, toggle arrow length (constant/speed).\n"
                                 "g, toggle rake mode (off/horiz/vert).\n"
                                 "s, switch scalar field.\n"
                                 "x, clear all seeds.\n"
                                 "q, <esc> - Quit\n",
                         filenames[0], filenames[1], filenames[2] );
                break;
        }
    }
}

// mouse callback to seed streamlines/pathlines
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        //getting cursor position
        glfwGetCursorPos(window, &xpos, &ypos);
        int gx = (int)xpos;
        int gy = vol_dim[1] - 1 - (int)ypos;
        gx = std::max(0, std::min(gx, vol_dim[0] - 1));
        gy = std::max(0, std::min(gy, vol_dim[1] - 1));

        int num_rake = 10;
        const char* mname = integration_method == 0 ? "Euler" : integration_method == 1 ? "RK2" : "RK4";
        fprintf(stderr, "mouseCallback: rake_mode=%d\n", rake_mode);

        if (en_streamline) {
            if (rake_mode) {
                bool horiz = rake_mode == 1;
                int step_x = horiz ? std::max(1, vol_dim[0] / (num_rake + 1)) : 0;
                int step_y = horiz ? 0 : std::max(1, vol_dim[1] / (num_rake + 1));
                int start_x = horiz ? step_x : gx;
                int start_y = horiz ? gy : step_y;
                for (int k = 0; k < num_rake; k++) {
                    int rx = std::min(start_x + k * step_x, vol_dim[0] - 1);
                    int ry = std::min(start_y + k * step_y, vol_dim[1] - 1);
                    streamline_seeds.push_back({rx, ry, integration_method});
                    computeStreamline(rx, ry);
                }
                fprintf(stderr, "Rake seeded %d streamlines %s at y=%d\n",
                    num_rake, horiz ? "horizontal" : "vertical", horiz ? gy : gx);
            } else {
                streamline_seeds.push_back({gx, gy, integration_method});
                computeStreamline(gx, gy);
                fprintf(stderr, "Streamline seeded at (%d,%d) method=%s\n", gx, gy, mname);
            }
        }
        if (en_pathline) {
            pathline_seeds.push_back({gx, gy, loaded_timestep, integration_method});
            computePathline(gx, gy, loaded_timestep);
            fprintf(stderr, "Pathline seeded at (%d,%d) t=%d method=%s\n", gx, gy, loaded_timestep, mname);
        }
    }
}

// glfw error callback
static void errorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

// data

void loadNextTimestep( void )
{
    loaded_timestep = ( loaded_timestep + 1 ) % num_timesteps;
    fprintf(stderr, "loadNextTimestep: now timestep %d, num_timesteps=%d\n", loaded_timestep, num_timesteps);
    fprintf(stderr, "  streamline_seeds.size=%zu, pathline_seeds.size=%zu\n",
        streamline_seeds.size(), pathline_seeds.size());
    DownloadScalarFieldAsTexture();
    // Recompute all streamlines/pathlines for the new timestep,
    // preserving each seed's original integration method
    streamlines.clear();
    for (size_t i = 0; i < streamline_seeds.size(); i++) {
        integration_method = streamline_seeds[i].method;
        computeStreamline(streamline_seeds[i].x, streamline_seeds[i].y);
    }
    pathlines.clear();
    for (size_t i = 0; i < pathline_seeds.size(); i++) {
        integration_method = pathline_seeds[i].method;
        computePathline(pathline_seeds[i].x, pathline_seeds[i].y, pathline_seeds[i].t);
    }
    fprintf(stderr, "loadNextTimestep: done, streamlines.size=%zu, pathlines.size=%zu\n",
        streamlines.size(), pathlines.size());
}


/*
 * load .gri dataset
 * This only reads the header information and calls the dat loader
 * For now we ignore the grid data and assume a rectangular grid
 */
void LoadData( char* base_filename )
{
    //reset
    reset_rendering_props();

    char filename[ 80 ];
    strcpy( filename, base_filename );
    strcat( filename, ".gri");

    fprintf( stderr, "loading grid file %s\n", filename );

    // open grid file, read only, binary mode
    FILE* fp = fopen( filename, "rb" );
    if ( fp == NULL ) {
        fprintf( stderr, "Cannot open file %s for reading.\n", filename );
        return;
    }

    // read header
    char header[ 40 ];
    fread( header, sizeof( char ), 40, fp );
    sscanf( header, "SN4DB %d %d %d %d %d %f",
            &vol_dim[ 0 ], &vol_dim[ 1 ], &vol_dim[ 2 ],
            &num_scalar_fields, &num_timesteps ,&timestep );

    fprintf( stderr, "dimensions: x: %d, y: %d, z: %d.\n", vol_dim[ 0 ], vol_dim[ 1 ], vol_dim[ 2 ] );
    fprintf( stderr, "additional info: # scalar fields: %d, # timesteps: %d, timestep: %f.\n", num_scalar_fields, num_timesteps, timestep );

    // read data
    char dat_filename[ 80 ];
    strcpy( dat_filename, base_filename );

    if( num_timesteps <= 1 ){

        strcat( dat_filename, ".dat");

    } else {

        strcat( dat_filename, ".00000.dat");

    }

    loaded_timestep = 0;
    LoadVectorData( base_filename );

    glfwSetWindowSize(window, vol_dim[ 0 ], vol_dim[ 1 ] );
    grid_data_loaded = true;
}

/*
 * load .dat dataset
 * loads vector and scalar fields
 */
void LoadVectorData( const char* filename )
{
    fprintf( stderr, "loading scalar file %s\n", filename );

    // open data file, read only, binary mode
    char ts_name[ 80 ];
    if( num_timesteps > 1 )
    {
        sprintf( ts_name, "%s.%.5d.dat", filename, 0 );
    }
    else
        sprintf( ts_name, "%s.dat",filename);

    FILE* fp = fopen( ts_name, "rb" );
    if ( fp == NULL ) {
        fprintf( stderr, "Cannot open file %s for reading.\n", filename );
        return;
    }
    else
    {
        fclose( fp );
    }

    data_size = vol_dim[ 0 ] * vol_dim[ 1 ] * vol_dim[ 2 ];

    if (!vector_array) {
        delete[] vector_array;
        vector_array = NULL;
    }
    // dim.xyz * vector.xyz * timesteps
    vector_array = new float[ data_size * 3 * num_timesteps];

    // read data
    if (!scalar_fields) {
        delete[] scalar_fields;
        scalar_fields = NULL;
        delete[] scalar_bounds;
        scalar_bounds = NULL;
    }
    // dim.xyz * scalarfields(2) * timesteps
    scalar_fields = new float[ data_size * num_scalar_fields * num_timesteps ];
    scalar_bounds = new float[ 2 * num_scalar_fields * num_timesteps ];

    int num_total_fields = num_scalar_fields + 3; // scalar fields + vec.xyz
    float *tmp = new float[ data_size * num_total_fields * num_timesteps ];

    for( int k = 0 ; k < num_timesteps; k++ )
    {
        char times_name[ 80 ];
        if( num_timesteps > 1 )
        {
            sprintf( times_name, "%s.%.5d.dat", filename, k );
            fp = fopen( times_name, "rb" );
        }
        else
            fp = fopen( ts_name, "rb" );
        // read scalar data
        fread( &tmp[k*data_size*num_total_fields], sizeof( float ), ( data_size * num_total_fields ), fp );

        // close file
        fclose( fp );

        // copy and scan for min and max values
        for( int  i = 0; i < num_scalar_fields; i++ ){

            float min_val = 99999.9f;
            float max_val = 0.0f;

            float avg = 0.0;

            int offset = i * data_size * num_timesteps;

            for( int j = 0; j < data_size; j++ ){

                float val = tmp[ j * num_total_fields + 3 + i + k*data_size*num_total_fields ];

                scalar_fields[ j + k*data_size + offset ] = val;

                if( toggle_xy ) {
                    // overwrite
                    if( i == 0 ){
                        vector_array[ 3*j + 0 + 3*k*data_size ] = tmp[ j * num_total_fields + 1 + k*data_size*num_total_fields ];//toggle x and y components in vector field
                        vector_array[ 3*j + 1 + 3*k*data_size ] = tmp[ j * num_total_fields + 0 + k*data_size*num_total_fields ];
                        vector_array[ 3*j + 2 + 3*k*data_size ] = tmp[ j * num_total_fields + 2 + k*data_size*num_total_fields ];
                    }
                } else {
                    // overwrite
                    if( i == 0 ){
                        vector_array[ 3*j + 0 + 3*k*data_size ] = tmp[ j * num_total_fields + 0 + k*data_size*num_total_fields ];
                        vector_array[ 3*j + 1 + 3*k*data_size ] = tmp[ j * num_total_fields + 1 + k*data_size*num_total_fields ];
                        vector_array[ 3*j + 2 + 3*k*data_size ] = tmp[ j * num_total_fields + 2 + k*data_size*num_total_fields ];
                    }
                }

                min_val = std::min( val, min_val );
                max_val = std::max( val, max_val );

                avg += scalar_fields[ offset + j + k*data_size ] / data_size;
            }
            scalar_bounds[ 2 * i     + k*num_scalar_fields*2 ] = min_val;
            scalar_bounds[ 2 * i + 1 + k*num_scalar_fields*2 ] = max_val;
        }

        // normalize
        for( int  i = 0; i < num_scalar_fields; i++ ){

            int offset = i * data_size * num_timesteps;

            float lower_bound = scalar_bounds[ 2 * i     + k*num_scalar_fields*2 ];
            float upper_bound = scalar_bounds[ 2 * i + 1 + k*num_scalar_fields*2 ];

            // scale between [0..1] where 1 is original zero
            // the boundary of the bigger abs border will be used to scale
            // meaning one boundary will likely not be hit i.e real scale
            // for -50..100 will be [0.25..1.0]
            if( lower_bound < 0.0 && upper_bound > 0.0 ){

                float scale = 0.5f / std::max( -lower_bound, upper_bound );

                for( int j = 0; j < data_size; j++ ){

                    scalar_fields[ offset + j + k*data_size ] = 0.5f + scalar_fields[ offset + j + k*data_size ] * scale;
                }
                scalar_bounds[ 2 * i     + k*num_scalar_fields*2 ] = 0.5f + scalar_bounds[ 2 * i     + k*num_scalar_fields*2 ] * scale;
                scalar_bounds[ 2 * i + 1 + k*num_scalar_fields*2 ] = 0.5f + scalar_bounds[ 2 * i + 1 + k*num_scalar_fields*2 ] * scale;


                // scale between [0..1]
            } else {

                float sign = upper_bound <= 0.0 ? -1.0f : 1.0f;

                float scale = 1.0f / ( upper_bound - lower_bound ) * sign;

                for( int j = 0; j < data_size; j++ ){

                    scalar_fields[ offset + j + k*data_size ] = ( scalar_fields[ offset + j + k*data_size ] - lower_bound ) * scale;
                }
                scalar_bounds[ 2 * i     + k*num_scalar_fields*2 ] = ( scalar_bounds[ 2 * i     + k*num_scalar_fields*2 ] + lower_bound ) * scale; //should be 0.0
                scalar_bounds[ 2 * i + 1 + k*num_scalar_fields*2 ] = ( scalar_bounds[ 2 * i + 1 + k*num_scalar_fields*2 ] + lower_bound ) * scale; //should be 1.0
            }
        }
    }
    delete[] tmp;
    DownloadScalarFieldAsTexture();

    scalar_data_loaded = true;
}


void DownloadScalarFieldAsTexture( void )
{
    fprintf( stderr, "downloading scalar field to 2D texture\n" );

    glEnable( GL_TEXTURE_2D );

    // generate and bind 2D texture
    glGenTextures( 1, &scalar_field_texture );
    glBindTexture( GL_TEXTURE_2D, scalar_field_texture );

    // set necessary texture parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

    int datasize = vol_dim[0] * vol_dim[1];
    //download texture in correct format
    glTexImage2D( GL_TEXTURE_2D, 0,  GL_INTENSITY16, vol_dim[ 0 ], vol_dim[ 1 ], 0, GL_LUMINANCE, GL_FLOAT, &scalar_fields[ (loaded_timestep + current_scalar_field * num_timesteps)*datasize ] );


    glDisable( GL_TEXTURE_2D );
}

char *textFileRead( char *fn ){

    FILE *fp;
    char *content = NULL;

    int count=0;

    if (!fn) {
        fp = fopen(fn,"rt");

        if (!fp) {

            fseek(fp, 0, SEEK_END);
            count = ftell(fp);
            rewind(fp);

            if (count > 0) {
                content = (char *)malloc(sizeof(char) * (count+1));
                count = fread(content,sizeof(char),count,fp);
                content[count] = '\0';
            }
            fclose(fp);
        }
    }
    return content;
}


// initializations
// init application
bool initApplication(int argc, char **argv)
{

    std::string version((const char *)glGetString(GL_VERSION));
    std::stringstream stream(version);
    unsigned major, minor;
    char dot;

    stream >> major >> dot >> minor;

    assert(dot == '.');
    if (major > 3 || (major == 2 && minor >= 0)) {
        std::cout << "OpenGL Version " << major << "." << minor << std::endl;
    } else {
        std::cout << "The minimum required OpenGL version is not supported on this machine. Supported is only " << major << "." << minor << std::endl;
        return false;
    }

    return true;
}

void reset_rendering_props( void )
{
    num_scalar_fields = 0;
}

// set up the scene
void setup() {
    LoadData( filenames[ 0 ] );
    loaded_file = 0;

    DownloadScalarFieldAsTexture();


    // compile & link shader
    vectorProgram.compileShader("shaders/vertex.vs");
    vectorProgram.compileShader("shaders/fragment.fs");
    vectorProgram.link();

    // make quad to render texture
    // see: vboquad.h and vboquad.cpp
    quad.init();

    glGenVertexArrays(1, &glyph_VAO);
    glGenBuffers(1, &glyph_VBO);
    glBindVertexArray(glyph_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, glyph_VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    glGenVertexArrays(1, &streamline_VAO);
    glGenBuffers(1, &streamline_VBO);
    glBindVertexArray(streamline_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, streamline_VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    glGenVertexArrays(1, &pathline_VAO);
    glGenBuffers(1, &pathline_VBO);
    glBindVertexArray(pathline_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, pathline_VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

}

// rendering
void render() {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Detect dt change & recompute all seeds (timestep change handled in loadNextTimestep)
    static float prev_dt = dt;
    if (prev_dt != dt) {
        fprintf(stderr, "Recomputing %zu streamlines, %zu pathlines (dt change)\n",
            streamline_seeds.size(), pathline_seeds.size());
        streamlines.clear();
        for (size_t i = 0; i < streamline_seeds.size(); i++) {
            integration_method = streamline_seeds[i].method;
            computeStreamline(streamline_seeds[i].x, streamline_seeds[i].y);
        }
        pathlines.clear();
        for (size_t i = 0; i < pathline_seeds.size(); i++) {
            integration_method = pathline_seeds[i].method;
            computePathline(pathline_seeds[i].x, pathline_seeds[i].y, pathline_seeds[i].t);
        }
        prev_dt = dt;
    }

    glEnable( GL_TEXTURE_2D );

    // draw the texture
    glBindTexture(GL_TEXTURE_2D, scalar_field_texture);
    vectorProgram.use();

    model = mat4(1);

    vectorProgram.setUniform("vertexColor", glm::vec4(0));
    vectorProgram.setUniform("model", model);

    vectorProgram.setUniform("colormapMode", colormap_mode);
    vectorProgram.setUniform("blendFactor", blend_factor);

    quad.render();

    glDisable( GL_TEXTURE_2D );

    // Reset colormap for overlays
    vectorProgram.setUniform("colormapMode", 0);

    // Draw glyphs
    if (en_arrow) drawGlyphs();

    // Draw streamlines (0=Euler=yellow, 1=RK2=cyan, 2=RK4=magenta)
    if (en_streamline) {
        for (size_t i = 0; i < streamlines.size(); i++) {
            if (streamlines[i].points.empty()) continue;
            glm::vec4 color = streamlines[i].method == 0 ? glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) :
                              streamlines[i].method == 1 ? glm::vec4(0.0f, 1.0f, 1.0f, 1.0f) :
                                                           glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
            vectorProgram.setUniform("vertexColor", color);
            std::vector<float> verts;
            for (size_t j = 0; j < streamlines[i].points.size(); j++) {
                float nx = 2.0f * streamlines[i].points[j].x / (vol_dim[0] - 1) - 1.0f;
                float ny = 2.0f * streamlines[i].points[j].y / (vol_dim[1] - 1) - 1.0f;
                verts.push_back(nx); verts.push_back(ny); verts.push_back(0.0f);
            }
            glBindBuffer(GL_ARRAY_BUFFER, streamline_VBO);
            glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_DYNAMIC_DRAW);
            glBindVertexArray(streamline_VAO);
            glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)(verts.size() / 3));
            glBindVertexArray(0);
        }
    }

    // Draw pathlines (0=Euler=yellow, 1=RK2=cyan, 2=RK4=magenta)
    if (en_pathline) {
        for (size_t i = 0; i < pathlines.size(); i++) {
            if (pathlines[i].points.empty()) continue;
            glm::vec4 color = pathlines[i].method == 0 ? glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) :
                              pathlines[i].method == 1 ? glm::vec4(0.0f, 1.0f, 1.0f, 1.0f) :
                                                         glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
            vectorProgram.setUniform("vertexColor", color);
            std::vector<float> verts;
            for (size_t j = 0; j < pathlines[i].points.size(); j++) {
                float nx = 2.0f * pathlines[i].points[j].x / (vol_dim[0] - 1) - 1.0f;
                float ny = 2.0f * pathlines[i].points[j].y / (vol_dim[1] - 1) - 1.0f;
                verts.push_back(nx); verts.push_back(ny); verts.push_back(0.0f);
            }
            glBindBuffer(GL_ARRAY_BUFFER, pathline_VBO);
            glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_DYNAMIC_DRAW);
            glBindVertexArray(pathline_VAO);
            glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)(verts.size() / 3));
            glBindVertexArray(0);
        }
    }

}

// entry point
int main(int argc, char** argv)
{
    // init variables
    view_width = 0;
    view_height = 0;

    toggle_xy = 0;

    en_arrow = false;
    en_streamline = false;
    en_pathline = false;
    sampling_rate = 15;
    dt = 0.1;

    reset_rendering_props();

    vector_array = NULL;
    scalar_fields = NULL;
    scalar_bounds = NULL;
    grid_data_loaded = false;
    scalar_data_loaded = false;
    current_scalar_field = 0;
    clearColor = 0;

    colormap_mode = 0;
    blend_factor = 0.5f;
    integration_method = 0;
    arrow_length_mode = 0;
    rake_mode = 0; // 0=off, 1=horizontal, 2=vertical

    filenames[ 0 ] = "data/block/c_block";
    filenames[ 1 ] = "data/tube/tube";
    filenames[ 2 ] = "data/hurricane/hurricane_p_tc";



    // set glfw error callback
    glfwSetErrorCallback(errorCallback);

    // init glfw
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    // init glfw window
    window = glfwCreateWindow(gWindowWidth, gWindowHeight, "AMCS/CS247 Scientific Visualization", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // set GLFW callback functions
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetFramebufferSizeCallback(window, frameBufferCallback);

    // make context current (once is sufficient)
    glfwMakeContextCurrent(window);

    // get the frame buffer size
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    // init the OpenGL API (we need to do this once before any calls to the OpenGL API)
    gladLoadGL();

    // init our application
    if (!initApplication(argc, argv)) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }


    // set up the scene
    setup();

    // print menu
    keyCallback(window, GLFW_KEY_BACKSLASH, 0, GLFW_PRESS, 0);

    // start traversing the main loop
    // loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // clear frame buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render one frame
        render();

        // swap front and back buffers
        glfwSwapBuffers(window);

        // poll and process input events (keyboard, mouse, window, ...)
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}

// ============================================================
// Helper functions for vector field access
// ============================================================

glm::vec2 getVectorAt(int x, int y, int t) {
    int idx = (t * data_size + y * vol_dim[0] + x) * 3;
    return glm::vec2(vector_array[idx], vector_array[idx + 1]);
}

glm::vec2 getVectorBilinear(float fx, float fy, int t) {
    if (fx < 0 || fx >= vol_dim[0] - 1 || fy < 0 || fy >= vol_dim[1] - 1)
        return glm::vec2(0.0f);
    int x0 = (int)fx, y0 = (int)fy;
    int x1 = x0 + 1, y1 = y0 + 1;
    float tx = fx - x0, ty = fy - y0;
    glm::vec2 v00 = getVectorAt(x0, y0, t);
    glm::vec2 v10 = getVectorAt(x1, y0, t);
    glm::vec2 v01 = getVectorAt(x0, y1, t);
    glm::vec2 v11 = getVectorAt(x1, y1, t);
    glm::vec2 v0 = v00 * (1.0f - tx) + v10 * tx;
    glm::vec2 v1 = v01 * (1.0f - tx) + v11 * tx;
    return v0 * (1.0f - ty) + v1 * ty;
}

glm::vec2 getVectorTrilinear(float fx, float fy, float ft) {
    if (ft < 0 || ft >= num_timesteps - 1)
        return glm::vec2(0.0f);
    int t0 = (int)ft, t1 = t0 + 1;
    float tt = ft - t0;
    glm::vec2 v0 = getVectorBilinear(fx, fy, t0);
    glm::vec2 v1 = getVectorBilinear(fx, fy, t1);
    return v0 * (1.0f - tt) + v1 * tt;
}

// ============================================================
// Integration helpers
// ============================================================

static void integratePts(std::vector<glm::vec2>& pts,
    float sx, float sy, float st, float dir, bool is_pathline, int method)
{
    float fx = sx, fy = sy, ft = st;
    float acc_len = 0.0f, max_len = 500.0f;
    for (int step = 0; step < 10000; step++) {
        glm::vec2 v = is_pathline ? getVectorTrilinear(fx, fy, ft)
                                  : getVectorBilinear(fx, fy, (int)st);
        float speed = glm::length(v);
        if (speed < 1e-6f) break;

        float nx, ny;
        if (method == 0) {
            nx = fx + v.x * dt * dir;
            ny = fy + v.y * dt * dir;
        } else if (method == 1) {
            float mx = fx + v.x * dt * dir * 0.5f;
            float my = fy + v.y * dt * dir * 0.5f;
            glm::vec2 v_mid = is_pathline
                ? getVectorTrilinear(mx, my, ft + dt * dir * 0.5f)
                : getVectorBilinear(mx, my, (int)st);
            nx = fx + v_mid.x * dt * dir;
            ny = fy + v_mid.y * dt * dir;
        } else {
            // RK4
            float half = 0.5f * dt * dir;
            glm::vec2 k1 = v;
            glm::vec2 k2 = is_pathline
                ? getVectorTrilinear(fx + half * k1.x, fy + half * k1.y, ft + half)
                : getVectorBilinear(fx + half * k1.x, fy + half * k1.y, (int)st);
            glm::vec2 k3 = is_pathline
                ? getVectorTrilinear(fx + half * k2.x, fy + half * k2.y, ft + half)
                : getVectorBilinear(fx + half * k2.x, fy + half * k2.y, (int)st);
            glm::vec2 k4 = is_pathline
                ? getVectorTrilinear(fx + dt * dir * k3.x, fy + dt * dir * k3.y, ft + dt * dir)
                : getVectorBilinear(fx + dt * dir * k3.x, fy + dt * dir * k3.y, (int)st);
            nx = fx + (dt * dir / 6.0f) * (k1.x + 2.0f * k2.x + 2.0f * k3.x + k4.x);
            ny = fy + (dt * dir / 6.0f) * (k1.y + 2.0f * k2.y + 2.0f * k3.y + k4.y);
        }

        if (nx < 0 || nx >= vol_dim[0] - 1 || ny < 0 || ny >= vol_dim[1] - 1) break;

        if (is_pathline) {
            ft += dt * dir;
            if (ft < 0 || ft >= num_timesteps - 1) break;
        }

        float step_len = std::sqrt((nx - fx) * (nx - fx) + (ny - fy) * (ny - fy));
        acc_len += step_len;
        if (acc_len > max_len) break;

        fx = nx; fy = ny;
        pts.push_back(glm::vec2(fx, fy));
    }
}

static void getStreamlinePts(std::vector<glm::vec2>& pts, int x, int y, int method) {
    pts.clear();
    std::vector<glm::vec2> fwd, bwd;
    integratePts(fwd, (float)x, (float)y, (float)loaded_timestep, 1.0f, false, method);
    integratePts(bwd, (float)x, (float)y, (float)loaded_timestep, -1.0f, false, method);
    pts.reserve(bwd.size() + 1 + fwd.size());
    for (int i = (int)bwd.size() - 1; i >= 0; i--) pts.push_back(bwd[i]);
    pts.push_back(glm::vec2((float)x, (float)y));
    pts.insert(pts.end(), fwd.begin(), fwd.end());
}

static void getPathlinePts(std::vector<glm::vec2>& pts, int x, int y, int t, int method) {
    pts.clear();
    std::vector<glm::vec2> fwd, bwd;
    integratePts(fwd, (float)x, (float)y, (float)t, 1.0f, true, method);
    integratePts(bwd, (float)x, (float)y, (float)t, -1.0f, true, method);
    pts.reserve(bwd.size() + 1 + fwd.size());
    for (int i = (int)bwd.size() - 1; i >= 0; i--) pts.push_back(bwd[i]);
    pts.push_back(glm::vec2((float)x, (float)y));
    pts.insert(pts.end(), fwd.begin(), fwd.end());
}

// ============================================================
// Filled stubs
// ============================================================

void computeStreamline(int x, int y)
{
    StreamlineData sd;
    sd.method = integration_method;
    getStreamlinePts(sd.points, x, y, sd.method);
    fprintf(stderr, "SL seed(%d,%d) fwd+bwd+seed=%zu pts method=%s\n", x, y, sd.points.size(), sd.method == 0 ? "Euler" : sd.method == 1 ? "RK2" : "RK4");
    glm::vec2 v0 = getVectorBilinear((float)x, (float)y, loaded_timestep);
    fprintf(stderr, "  v0=(%.3f,%.3f) len=%.3f\n", v0.x, v0.y, glm::length(v0));
    if (sd.points.size() > 0) {
        float nx0 = 2.0f * sd.points[0].x / (vol_dim[0]-1) - 1.0f;
        float ny0 = 2.0f * sd.points[0].y / (vol_dim[1]-1) - 1.0f;
        size_t last = sd.points.size()-1;
        float nx1 = 2.0f * sd.points[last].x / (vol_dim[0]-1) - 1.0f;
        float ny1 = 2.0f * sd.points[last].y / (vol_dim[1]-1) - 1.0f;
        fprintf(stderr, "  NDC: first=(%.3f,%.3f) last=(%.3f,%.3f)\n", nx0, ny0, nx1, ny1);
    }
    streamlines.push_back(sd);
}

void computePathline(int x, int y, int t)
{
    PathlineData pd;
    pd.method = integration_method;
    getPathlinePts(pd.points, x, y, t, pd.method);
    pathlines.push_back(pd);
}

void drawGlyphs() {
    std::vector<float> verts;
    int w = vol_dim[0], h = vol_dim[1];
    float arrow_len = arrow_length_mode == 0 ? 0.04f : 0.0f;

    // Find max speed for speed-proportional mode
    float max_speed = 1.0f;
    if (arrow_length_mode == 1) {
        max_speed = 0.0f;
        for (int y = 0; y < h; y += sampling_rate) {
            for (int x = 0; x < w; x += sampling_rate) {
                glm::vec2 v = getVectorAt(x, y, loaded_timestep);
                float s = glm::length(v);
                if (s > max_speed) max_speed = s;
            }
        }
        if (max_speed < 1e-6f) max_speed = 1.0f;
    }

    for (int y = 0; y < h; y += sampling_rate) {
        for (int x = 0; x < w; x += sampling_rate) {
            glm::vec2 v = getVectorAt(x, y, loaded_timestep);
            float speed = glm::length(v);
            if (speed < 1e-6f) continue;

            float cx = 2.0f * x / (w - 1) - 1.0f;
            float cy = 2.0f * y / (h - 1) - 1.0f;
            float dx = v.x / speed, dy = v.y / speed;

            float alen = arrow_length_mode == 0
                ? arrow_len
                : (speed / max_speed) * 0.05f;

            float tx = cx + dx * alen, ty = cy + dy * alen;
            float bx = cx - dx * alen, by = cy - dy * alen;

            // Shaft: tail → tip
            verts.push_back(bx); verts.push_back(by); verts.push_back(0.0f);
            verts.push_back(tx); verts.push_back(ty); verts.push_back(0.0f);

            // Arrowhead
            float hlen = alen * 0.4f, hw = hlen * 0.6f;
            float px = -dy, py = dx; // perpendicular
            float hl = tx - dx * hlen + px * hw;
            float hr = tx - dx * hlen - px * hw;
            float hl_y = ty - dy * hlen + py * hw;
            float hr_y = ty - dy * hlen - py * hw;

            verts.push_back(tx); verts.push_back(ty); verts.push_back(0.0f);
            verts.push_back(hl); verts.push_back(hl_y); verts.push_back(0.0f);
            verts.push_back(tx); verts.push_back(ty); verts.push_back(0.0f);
            verts.push_back(hr); verts.push_back(hr_y); verts.push_back(0.0f);
        }
    }

    if (verts.empty()) return;
    vectorProgram.setUniform("vertexColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    glBindBuffer(GL_ARRAY_BUFFER, glyph_VBO);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_DYNAMIC_DRAW);
    glBindVertexArray(glyph_VAO);
    glDrawArrays(GL_LINES, 0, (GLsizei)(verts.size() / 3));
    glBindVertexArray(0);
}
