package vyglab.argeo.jni;

import android.graphics.Bitmap;

public interface SnapshotListener
{
    void onSnapshotAvailable(Bitmap snapshot);

    void onARBufferSnapshotAvailable(
            Bitmap reality_snapshot,
            Bitmap virtuality_snapshot,
            Bitmap mixed_reality_snapshot);

    void onTTARSnapshotAvailable(
            Bitmap reality_snapshot,
            Bitmap ttar_snapshot,
            Camera camera);
}
