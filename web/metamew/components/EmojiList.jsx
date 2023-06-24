import { useState } from "react";
import { FlatList, Platform, Pressable } from "react-native";
import { Image } from "react-native";
import { keyboardProps } from "react-native-web/dist/cjs/modules/forwardedProps";

export default function EmojiList({ onSelect, onCloseModal }) {
  cosnt[emoji] = useState([
    require("../assets/adaptive-icon.png"),
    require("../assets/icon.png"),
    require("../assets/splash.png"),
  ]);

  return (
    <FlatList
      horizontal
      showsHorizontalScrollIndicator={Platform.OS === "web"}
      data={emoji}
      contentContainerStyle={styles.listContainer}
      renderItem={({ item, index }) => {
        return (
          <Pressable
            onPress={() => {
              onSelect(item);
              onCloseModal();
            }}
          >
            <Image source={item} key={index} style={styles.image} />
          </Pressable>
        );
      }}
    />
  );
}

const styles = StyleSheet.create({
  listContainer: {
    borderTopRightRadius: 10,
    borderTopLeftRadius: 10,
    paddingHorizontal: 20,
    flexDirection: "row",
    alignItems: "center",
    justifyContent: "space-between",
  },
  image: {
    width: 100,
    height: 100,
    marginRight: 20,
  },
});
