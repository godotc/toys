const {describe} = require('node:test');
const sum = require('./sum')


describe("add", () => {
	test('1+2=3', () => {
		expect(sum(1, 2)).toBe(3);
	});
	test('1+3=4', () => {
		expect(sum(1, 3)).toBe(4)
	});
});
