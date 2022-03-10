import os, zipfile,sys,time,shutil
def make_zip(src_dir,dst_dir,output_filename):
  zipf = zipfile.ZipFile(output_filename, 'w',zipfile.ZIP_DEFLATED)
  pre_len = len(os.path.dirname(src_dir))
  for parent, dirnames, filenames in os.walk(src_dir):
    for filename in filenames:
      pathfile = os.path.join(parent, filename)
      arcname = pathfile[pre_len:].strip(os.path.sep)
      zipf.write(pathfile, arcname)
  zipf.close()
  shutil.move(f'{output_filename}',dst_dir)

def unzip_file(zip_src, dst_dir):    
    fz = zipfile.ZipFile(zip_src, 'r')
    for file in fz.namelist():
        fz.extract(file, dst_dir)

def isexists_dir_Create(path):
     if not os.path.exists(path):
        os.makedirs(path)

src_path = os.path.expanduser('~\\AppData\\Roaming\\EldenRing')
dst_path = os.path.expanduser('~\\Documents\\EldenRingBackFile')
zip_name = f'{time.strftime("%Y-%m-%d %H时%M分%S秒", time.localtime())}.zip'

isexists_dir_Create(dst_path)

while True:
  try:
    cmd = int(input('输入数字以继续操作(按 0 退出程序):\n1. 快速存档\n2. 快速换档\n'))
    if cmd not in range(3):
      print('命令无效, 请重新输入')
      continue
  except Exception:
    print('输入有误')
    continue
  break
if cmd==0:
  sys.exit(0)
elif cmd==1:
  make_zip(src_path,dst_path,zip_name)
  print('快速存档完成,存档位于%s'%dst_path)
  msg = input('按回车键退出...')
  sys.exit(0)
else:
  print('以下为您之前的存档, 请选择一个回档(输入存档前面的数字,按 0 退出程序):')
  for parent, dirnames, filenames in os.walk(dst_path):
    for i in range(len(filenames)):
      print(str(i+1)+'. '+filenames[i])
  while True:
    try:
      cmd = int(input())
      if cmd not in range(len(filenames)+1):
        print('无此存档, 请重新输入')
        continue
    except Exception:
      print('输入有误, 请重新输入')
      continue
    break
  if cmd==0:
    sys.exit(0)
  else:
    unzip_file(dst_path+f'\\{filenames[cmd-1]}',src_path.replace('EldenRing',''))
    print('换挡完成')
  msg = input('按回车键退出...')
  sys.exit(0)
    