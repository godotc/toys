// swagger.js

const swaggerDefinition = {
    openapi: '3.0.0',
    info: {
        title: 'User Authentication API',
        version: '1.0.0',
        description: 'Endpoints to test the user registration and login',
    },
    servers: [
        {
            url: 'http://localhost:8080/api',
            description: 'Local server',
        },
    ],
};

const options = {
    swaggerDefinition,
    // Paths to files containing OpenAPI definitions
    apis: ['./src/user.js'],
};

export default options;
