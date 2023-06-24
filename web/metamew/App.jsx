import { StatusBar } from "expo-status-bar";
import { StyleSheet, Image, View } from "react-native";
import ImageViewer from "./components/ImageViewer";
import Button from "./components/Button";
import * as ImagePicker from "expo-image-picker";
import { useState } from "react";
import IconButton from "./components/IconButton";
import CircleButton from "./components/CircleButton";
import EmojiPicker from "./components/EmojiPicker";
import EmojiList from "./components/EmojiList";
import EmojiSticker from "./components/EmojiSticker";

const BG = require("./assets/splash.png");

export default function App() {
  const [selectedImage, setSelectedImage] = useState(null);
  const [isShowAppOptions, setIsShowAppOption] = useState(false);
  const [isModalVisible, setIsModalVisible] = useState(false);
  const [pickedEmoji, setPickedEmoji] = useState(null);

  const pickImageAsync = async () => {
    let result = await ImagePicker.launchImageLibraryAsync({
      allowsEditing: true,
      quality: 1,
    });

    if (!result.canceled) {
      console.log(result);
      setSelectedImage(result.assets[0].uri);
      setIsShowAppOption(true);
    } else {
      alert("Please select a image.");
    }
  };

  const onReset = () => {
    setIsShowAppOption(false);
  };

  const onAddSticker = () => {
    // we will implement this later
    setIsModalVisible(ture);
  };

  const onSaveImageAsync = async () => {
    // we will implement this later
  };
  const onModalClose = () => {
    setIsModalVisible(false);
  };

  //<Text style={{ color: "#f00" }}>Hello World</Text>;
  return (
    <View style={styles.container}>
      <View style={styles.imageContainer}>
        <ImageViewer placeHolderImage={BG} selectedImage={selectedImage} />

        {pickedEmoji === null ? (
          <EmojiSticker imageSize={40} stickerSource={pickedEmoji} />
        ) : null}
      </View>
      {isShowAppOptions ? (
        <View />
      ) : (
        <View style={styles.footerContainer}>
          <Button
            theme={"primary"}
            label={"Choose a photo"}
            onPress={pickImageAsync}
          />

          <Button
            label={"Use this photo"}
            onPress={() => setIsShowAppOption(true)}
          />
        </View>
      )}

      {isShowAppOptions ? (
        <View style={styles.optionsContainer}>
          <view style={styles.optionRow}>
            <IconButton icon="refresh" label="Reset" onPress={onReset} />
            <CircleButton onPress={onAddSticker} />
            <IconButton
              icon="save-alt"
              label="Save"
              onPress={onSaveImageAsync}
            />
          </view>
        </View>
      ) : (
        <></>
      )}

      <EmojiPicker isVisiable={isModalVisible} onclose={onModalClose}>
        <EmojiList onSelect={setPickedEmoji} onCloseModal={onModalClose} />
      </EmojiPicker>

      <StatusBar style="auto" />
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: "#25292e",
    alignItems: "center",
    justifyContent: "center",
  },
  imageContainer: {
    flex: 1,
    paddingTop: 58,
  },
  image: {
    width: 320,
    height: 440,
    borderRadius: 18,
  },
  footerContainer: {
    flex: 1 / 3,
    alignItems: "center",
  },
  optionsContainer: {
    position: "absolute",
    bottom: 80,
  },
  optionRow: {
    alignItems: "center",
    flexDirection: "row",
  },
});
