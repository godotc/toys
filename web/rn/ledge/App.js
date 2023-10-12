import { StatusBar } from 'expo-status-bar';
import { Pressable, StyleSheet, Text, View } from 'react-native';
import { Button } from 'react-native-web';
import { RecoredPage } from './src/recored_page';
import { LostsOffGreetings } from './src/greetings';

export default function App() {

	return (
		<View style={styles.container}>
			<Text>Open up App.js to start working on your app!</Text>
			<Pressable style={styles.btnTest}> <Text>Hello World</Text> </Pressable >
			<RecoredPage />
			<StatusBar> Bar</StatusBar>
		</View >
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
