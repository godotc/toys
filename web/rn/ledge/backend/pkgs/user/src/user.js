import { user } from '@ledge/db';
import { generateJWT } from './jwt.js';
import { error } from 'console';


// import { RequestHandler } from './types.js';

// const newUserProps = {
//     name: 'John Doe',
//     email: 'john@example.com',
//     password: 'securepassword',
// };


// user.createUser(newUserProps)
//     .then((createdUser) => {
//         console.log('User created:', createdUser);
//     })
//     .catch((error) => {
//         console.error('Error:', error);
//     });


/** @type {RequestHandler}*/
export const login = (req, resp) => {
    try {
        const body = req.body // undefined
        const email = body.email
        const password = body.password
        if (email === '') throw "email is empty"
        if (password === '') throw "password is empty"

        user.getUserByEmail(email).then((user) => {
            if (user.email === email && user.password === password) {
                // return a token
                const token = generateJWT(user.id)
                resp.json({ token })
            }
        })

    } catch (error) {
        console.log(error)
        resp.status(400).json({ error: 'Invalid JSON data' });
    }
}

/** @type {RequestHandler}*/
export const register = (req, resp) => {
    try {
        const body = req.body // undefined
        const email = body.email
        const password = body.password
        if (email === '') throw "email is empty"
        if (password === '') throw "password is empty"

        user.createUser({ email, password })
            .then((user) => {
                const token = generateJWT(user.id)
                resp.json({ token });
            }).catch((error) => {
                console.error(error)
                resp.status(400).json({ error: "Create user failed" })
            });
    } catch (error) {
        console.log(error)
        resp.status(400).json({ error: 'Invalid request data' });
    }
}