import express from 'express'

const app = express()

import { user } from '@ledge/db';



app.post('/user/add', (req, resp) => {


    /**@type('user').newUserProps */
    const data = req.body;

    const createdUser = user.createUser({
        email: data['email'],
        name: data['name'],
        password: data['password']
    })

    resp.send({ msg: "Create success", id: createdUser.id })

})
