# ImageNet train on ResNet
## Environment Prepare
Downloads the files from Image-Net and processes them into TFRecords but does not upload them to GCS.
```
export IMAGENET_HOME=/home/benchmark/imagenet
export TF_RECORDS_HOME=/home/wufeiyang/Env/imagenet

python imagenet_to_gcs.py \
  --raw_data_dir=$IMAGENET_HOME \
  --local_scratch_dir=$TF_RECORDS_HOME/tf_records \
  --nogcs_upload
```
然后这个文件GG了，于是不看这个了。