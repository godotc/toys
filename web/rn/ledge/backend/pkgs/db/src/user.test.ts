

import { expect, describe, it, test, beforeAll, afterAll } from "@jest/globals"
import { createUser, deleteUserByEmail, getUserByEmail, updatePassword } from "./user";

import { faker } from '@faker-js/faker'
import { User } from "@prisma/client";


// Test cases using Jest or any other testing framework
describe("User CRUD Operations", () => {
    let defaultUser: {
        name: string,
        email: string,
        password: string
    }

    beforeAll(async () => {
        defaultUser = {
            name: faker.person.fullName(),
            email: faker.internet.email(),
            password: faker.person.firstName()
        };
    });

    afterAll(async () => {
        // Clean up after all tests, e.g., delete test data
    });

    it("should create a new user", async () => {
        const user = await createUser(defaultUser);
        expect(user).toBeDefined();
    });

    it("should get a user by email", async () => {
        const user = await getUserByEmail(defaultUser.email);
        expect(user).toBeDefined();
    });

    it("should update a user's password", async () => {
        const newPassWord = faker.person.fullName();
        const updatedUser = await updatePassword(defaultUser.email, defaultUser.password, newPassWord);
        expect(defaultUser.name == updatedUser.name && updatedUser.password === newPassWord)
    });

    it("should delete a user", async () => {
        await deleteUserByEmail(defaultUser.email);
        const user = await getUserByEmail("john@example.com");
        expect(user).toBeNull();
    });
});
