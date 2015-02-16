package org.kipr.kovan;

public class Loader {
  static {
      try {
        System.loadLibrary("kovan");
      } catch (final UnsatisfiedLinkError e) {
        System.err.println("Failed to load kovan native library.\n" + e);
      }
  }
}