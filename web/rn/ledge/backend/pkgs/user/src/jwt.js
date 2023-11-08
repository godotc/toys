

import dotenv from 'dotenv';
dotenv.config()

import jwt from 'jsonwebtoken'

/**
 * @param {number} id 
 * @returns  {string} jwt
 */
export const generateJWT = (id) => {
    return jwt.sign({ id: id.toString() }, process.env.JWT_SECRET, { expiresIn: '1800s' })
}

// test
const validateJWT = (token) => {
    jwt.verify(token, process.env.JWT_SECRET, (err, payload) => {

        if (err) {
            return { error: "Token is invalid" }
        }

        console.log(payload)
        console.log(payload.id)
        return payload.id;
    })
}

// Middleware to verify the token for protected routes
const authenticateToken = (req, resp, next) => {
    const token = req.headers['authorization'];

    if (token) {
        jwt.verify(token, secretKey, (err, payload) => {
            if (err) {
                return resp.status(403).json({ error: 'Token is invalid' });
            }
            req.userid = payload.id;
            next();
        });
    } else {
        resp.status(401).json({ error: 'Token not provided' });
    }
};


// console.log(process.env.JWT_SECRET)
// console.log(generateJWT(1))
// const token = generateJWT(1);
// validateJWT(token)