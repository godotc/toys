import { StatusBar } from 'expo-status-bar';
import { Pressable, StyleSheet, Text, View, useColorScheme } from 'react-native';
import { RecoredPage } from './src/recored_page';
import { Greeting } from './src/greetings';
import { useState } from 'react';
import { UseColor } from "react-native"
import { NavigationContainer } from '@react-navigation/native'
import { createStackNavigator } from '@react-navigation/stack'
import { createBottomTabNavigator } from '@react-navigation/bottom-tabs'
import Login from './src/login';


// const Stack = createStackNavigator();
const Tab = createBottomTabNavigator();


export default function App() {

	const [firstIn, setFirstIn] = useState(false)
	const theme = useColorScheme();
	const isDarkTheme = theme === 'dark';


	return <Login />

	console.log(firstIn)
	if (firstIn) return <Greeting setFirstIn={setFirstIn} />

	return (
		<NavigationContainer>
			<Tab.Navigator >
				<Tab.Screen name="App" component={RecoredPage} />
			</Tab.Navigator>
		</NavigationContainer>

	);

}

const styles = StyleSheet.create({
	container: {
		flex: 1,
		backgroundColor: '#fff',
		alignItems: 'center',
		justifyContent: 'center',
	},
	btnTest: {
		backgroundColor: "#581",
		padding: 3,
		borderRadius: 14,
		alignContent: 'center',
		justifyContent: "center",
	}
});
