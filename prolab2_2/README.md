# Araç Kontrol Sistemi

Bu proje, bir araç kontrol sisteminin simülasyonunu içerir. Sistem, emniyet kemeri kontrolü, motor kontrolü, sıcaklık kontrolü, far kontrolü, yakıt kontrolü ve kapı kontrolü gibi temel araç fonksiyonlarını simüle eder.

## Programlama Dili

Bu proje Arduino Programming Language (Arduino C++) kullanılarak geliştirilmiştir. Arduino C++, C++ programlama dilinin özel bir versiyonudur ve Arduino platformu için özelleştirilmiş kütüphaneler ve fonksiyonlar içerir. Temel özellikleri:

- C++ tabanlı bir programlama dili
- Arduino IDE ile geliştirme
- Özel Arduino kütüphaneleri (LiquidCrystal, vb.)
- Basitleştirilmiş giriş/çıkış işlemleri
- Mikrodenetleyici tabanlı programlama

## Senaryolar

### Senaryo 1: Motor Çalıştırma ve Emniyet Kemeri Kontrolü

#### Şartlar
- Motor butonuna basıldığında emniyet kemeri takılı değilse motor çalışmamalıdır.

#### Senaryo Detayları
- Eğer kemer takılı değilse:
  - Buzzer öter
  - Kırmızı LED yanar
  - LCD ekranda "Emniyet Kemeri Takılı Değil!" mesajı görünür
- Kemer takıldığında:
  - Buzzer ötmeyi bırakır
  - LED söner
  - LCD ekran temizlenir
- Motorun tekrar çalıştırılması için motor butonuna tekrar basılmalıdır

### Senaryo 2: Sıcaklık Kontrolü

#### Şartlar
- Araç içi sıcaklık 25°C'nin üzerine çıktığında klima otomatik olarak çalışmalıdır.

#### Senaryo Detayları
- Sıcaklık 25°C'nin üzerine çıktığında:
  - Klima LED'i yanar
  - LCD ekranda sıcaklık değeri gösterilir
- Sıcaklık 25°C'nin altına düştüğünde:
  - Klima LED'i söner
  - LCD ekranda sıcaklık değeri gösterilir

### Senaryo 3: Far Kontrolü

#### Şartlar
- Ortam ışığı azaldığında farlar otomatik olarak yanmalıdır.

#### Senaryo Detayları
- Işık seviyesi düşük olduğunda:
  - Mavi LED (Far) yanar
  - LCD ekranda "FARLAR: ACIK" mesajı görünür
- Işık seviyesi yeterli olduğunda:
  - Mavi LED (Far) söner
  - LCD ekranda "FARLAR: KAPALI" mesajı görünür

### Senaryo 4: Yakıt Kontrolü

#### Şartlar
- Yakıt seviyesi kritik seviyenin altına düştüğünde uyarı verilmelidir.

#### Senaryo Detayları
- Yakıt bittiğinde:
  - Motor durur
  - Tüm LED'ler söner
  - LCD ekranda "YAKIT BITTI" ve "MOTOR DURDU" mesajları görünür
- Yakıt kritik seviyede (<%5) olduğunda:
  - Sarı LED yanıp söner
  - LCD ekranda "KRITIK: YAKIT AZ" ve yakıt seviyesi gösterilir
- Yakıt düşük seviyede (<%10) olduğunda:
  - Sarı LED sürekli yanar
  - LCD ekranda "UYARI: YAKIT DUSUK" ve yakıt seviyesi gösterilir
- Yakıt normal seviyede olduğunda:
  - Sarı LED söner
  - LCD ekranda "YAKIT NORMAL" ve yakıt seviyesi gösterilir

### Senaryo 5: Kapı Kontrolü

#### Şartlar
- Kapı açıkken motor çalışmamalıdır.

#### Senaryo Detayları
- Kapı açık olduğunda:
  - RGB LED kırmızı ve mavi yanar
  - Motor durur
  - LCD ekranda "UYARI: KAPI ACIK" ve "MOTOR CALISMAZ" mesajları görünür
- Kapı kapalı olduğunda:
  - RGB LED söner
  - LCD ekranda "KAPI: KAPALI" ve "MOTOR HAZIR" mesajları görünür
  - Motor butonuna basılarak motor çalıştırılabilir

### Senaryo 6: Tüm Sistem

#### Şartlar
- Tüm sistemlerin birlikte çalışması ve birbirini etkilemesi.

#### Senaryo Detayları
- Tüm sensörler ve kontroller aktif
- Her sistem birbirini etkiler:
  - Kapı açıkken motor çalışmaz
  - Emniyet kemeri takılı değilken motor çalışmaz
  - Yakıt bittiğinde motor çalışmaz
  - Sıcaklık yüksekse klima çalışır
  - Işık azsa farlar yanar
- LCD ekranda aktif sistemlerin durumları gösterilir

## Kullanılan Bileşenler

- Arduino Mega
- LCD Ekran (16x2)
- Butonlar:
  - Motor Başlatma Butonu
  - Emniyet Kemeri Butonu
- Sensörler:
  - Sıcaklık Sensörü (LM35)
  - Işık Sensörü (LDR)
  - Yakıt Sensörü (Potansiyometre)
- LED'ler:
  - Kırmızı LED (Emniyet Kemeri)
  - Mavi LED (Far)
  - Sarı LED (Yakıt)
  - RGB LED (Kapı)
  - Yeşil LED (Ana Motor)
  - Mavi LED (Klima)
- Buzzer
- Dirençler (220Ω)

## Bağlantılar

- Motor Butonu: Pin 2
- Emniyet Kemeri Butonu: Pin 3
- Kapı Anahtarı: Pin 4
- Buzzer: Pin 11
- Kırmızı LED: Pin 5
- Mavi LED: Pin 6
- Sarı LED: Pin 7
- Kapı LED (RGB): Pin 8, 9, 10
- Ana Motor LED: Pin 12
- Klima LED: Pin 13
- LCD Ekran:
  - RS: Pin 22
  - EN: Pin 23
  - D4-D7: Pin 24-27
- Sensörler:
  - Sıcaklık: A0
  - Işık: A1
  - Yakıt: A2

## Kurulum

1. Arduino IDE'yi yükleyin
2. Projeyi bilgisayarınıza indirin
3. `sketch.ino` dosyasını Arduino IDE'de açın
4. Gerekli kütüphaneleri yükleyin:
   - LiquidCrystal
5. Kodu Arduino Mega'ya yükleyin

## Kullanım

1. Sistemi başlatın
2. Test menüsünden senaryo 1'i seçin
3. Emniyet kemeri butonuna basarak kemeri takıp çıkarın
4. Motor butonuna basarak motoru çalıştırmayı deneyin
5. Sistemin davranışını gözlemleyin

## Test Senaryoları

1. Kemer Testi
2. Sıcaklık Testi
3. Far Testi
4. Yakıt Testi
5. Kapı Testi
6. Tüm Sistem

## Notlar

- Sistem Wokwi simülatöründe test edilmiştir
- Tüm bileşenler breadboard üzerinde bağlanmıştır
- Her bileşen için uygun dirençler kullanılmıştır 