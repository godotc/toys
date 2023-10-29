
import { user } from '@ledge/db';

const newUserProps = {
    name: 'John Doe',
    email: 'john@example.com',
    password: 'securepassword',
};


user.createUser(newUserProps)
    .then((createdUser) => {
        console.log('User created:', createdUser);
    })
    .catch((error) => {
        console.error('Error:', error);
    });